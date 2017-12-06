#include "ASTScraper.hpp"

#include <iostream>

#include <clang/AST/RecordLayout.h>
#include <clang/AST/DeclTemplate.h>
#include "clang/AST/Decl.h"

#include "MetaCPP/Type.hpp"

namespace metacpp {
	ASTScraper::ASTScraper(Storage* storage, const Configuration& config)
		: m_Config(config), m_Storage(storage)
	{
	}

	void ASTScraper::ScrapeTranslationUnit(const clang::TranslationUnitDecl* tuDecl)
	{
		ScrapeDeclContext(tuDecl, nullptr);
	}

	void ASTScraper::ScrapeDeclContext(const clang::DeclContext* ctxDecl, Type* parent)
	{
		for (clang::DeclContext::decl_iterator it = ctxDecl->decls_begin(); it != ctxDecl->decls_end(); ++it)
		{
			const clang::Decl* decl = *it;
			if (decl->isInvalidDecl())
				continue;

			auto namedDecl = clang::dyn_cast<clang::NamedDecl>(decl);
			if (namedDecl)
				ScrapeNamedDecl(namedDecl, parent);
		}
	}

	void ASTScraper::ScrapeNamedDecl(const clang::NamedDecl* namedDecl, Type* parent)
	{
		clang::Decl::Kind kind = namedDecl->getKind();
		switch (kind) {
		case clang::Decl::Kind::ClassTemplate:
		case clang::Decl::Kind::CXXConstructor:
		case clang::Decl::Kind::CXXDestructor:
		case clang::Decl::Kind::CXXMethod:
		default:
		{
			// Unhandled
			//std::string qualifiedName = namedDecl->getQualifiedNameAsString();
			//std::cout << "Unhandled! " << namedDecl->getDeclKindName() << ": " << qualifiedName << std::endl;
			break;
		}
		case clang::Decl::Kind::Namespace:
			ScrapeDeclContext(namedDecl->castToDeclContext(namedDecl), parent);
			break;
		case clang::Decl::Kind::CXXRecord:
			ScrapeCXXRecordDecl(clang::dyn_cast<clang::CXXRecordDecl>(namedDecl), parent);
			break;
		case clang::Decl::Kind::Field:
			ScrapeFieldDecl(clang::dyn_cast<clang::FieldDecl>(namedDecl), parent);
			break;
		}
	}

	Type* ASTScraper::ScrapeCXXRecordDecl(const clang::CXXRecordDecl* cxxRecordDecl, Type* parent)
	{
		if (cxxRecordDecl->isAnonymousStructOrUnion()) {
			ScrapeDeclContext(cxxRecordDecl, parent);
			return 0;
		}

		const clang::Type* cType = cxxRecordDecl->getTypeForDecl();
		metacpp::Type* type = ScrapeType(cType);

		if (type)
			type->setAccess(TransformAccess(cxxRecordDecl->getAccess()));

		return type;
	}

	std::vector<QualifiedType*> ASTScraper::ResolveCXXRecordTemplate(const clang::CXXRecordDecl* cxxRecordDecl, QualifiedName& qualifiedName)
	{
		std::string typeName = cxxRecordDecl->getQualifiedNameAsString();
		std::vector<QualifiedType*> templateArgs;

		// Template arguments
		auto templateDecl = llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(cxxRecordDecl);
		if (templateDecl) {
			typeName += "<";

			const clang::TemplateArgumentList& args = templateDecl->getTemplateArgs();

			for (int i = 0; i < args.size(); i++) {
				const clang::TemplateArgument& arg = args[i];
				if (i)
					typeName += ",";

				QualifiedType* qualifiedType = 0;
				switch (arg.getKind()) {
					case clang::TemplateArgument::Type:
						qualifiedType = ResolveQualType(arg.getAsType());
						break;
					default:
						std::cout << "Unsupported template argument!" << std::endl;
						break;
				}

				if (qualifiedType)
					typeName += qualifiedType->getQualifiedName(m_Storage);
				else
					typeName += "INVALID";

				templateArgs.push_back(qualifiedType);
			}

			typeName += ">";
		}

		qualifiedName = ResolveQualifiedName(typeName);

		return templateArgs;
	}

	Type* ASTScraper::ScrapeType(const clang::Type* cType)
	{
		QualifiedName qualifiedName;
		metacpp::TypeKind kind;
		std::vector<QualifiedType*> templateArgs;

		switch (cType->getTypeClass()) {
		case clang::Type::TypeClass::Builtin:
		{
			static clang::LangOptions lang_opts;
			static clang::PrintingPolicy printing_policy(lang_opts);

			auto builtin = cType->getAs<clang::BuiltinType>();
			std::string name = builtin->getName(printing_policy);
			if (name == "_Bool")
				name = "bool";
			qualifiedName = QualifiedName({}, name);
			kind = metacpp::TypeKind::PRIMITIVE;
			break;
		}
		case clang::Type::TypeClass::Record:
		{
			auto cxxRecordDecl = cType->getAsCXXRecordDecl();
			if (cxxRecordDecl->isThisDeclarationADefinition() == clang::VarDecl::DeclarationOnly)
				return 0; // forward declaration
			templateArgs = ResolveCXXRecordTemplate(cxxRecordDecl, qualifiedName);
			kind = cType->isStructureType() ? metacpp::TypeKind::STRUCT : metacpp::TypeKind::CLASS;
			break;
		}
		default:
			std::cout << "Unsupported TypeClass " << cType->getTypeClassName() << std::endl;
			return 0;
		}
		
		TypeID typeId = m_Storage->assignTypeID(qualifiedName.fullQualified());

		if (m_Storage->hasType(typeId))
			return m_Storage->getType(typeId);

		metacpp::Type* type = new metacpp::Type(typeId, qualifiedName);
		
		type->setSize(m_Context->getTypeSize(cType) / 8);
		type->setKind(kind);

		if (auto cxxRecordDecl = cType->getAsCXXRecordDecl()) {
			// Parse base classes
			for (auto it = cxxRecordDecl->bases_begin(); it != cxxRecordDecl->bases_end(); it++)
			{
				type->addBaseType(ResolveQualType(it->getType()), TransformAccess(it->getAccessSpecifier()));
			}

			// Scrape annotations
			std::vector<std::string> annotations = ScrapeAnnotations(cxxRecordDecl);
		}

		for (auto qt : templateArgs)
			type->addTemplateArgument(qt);

		m_Storage->addType(type);

		if (auto declCtx = cType->getAsTagDecl()->castToDeclContext(cType->getAsTagDecl()))
			ScrapeDeclContext(declCtx, type);

		return type;
	}

	void ASTScraper::ScrapeFieldDecl(const clang::FieldDecl* fieldDecl, Type* parent)
	{
		if (!parent)
			return;
		if (fieldDecl->isAnonymousStructOrUnion())
			return;

		metacpp::QualifiedName qualifiedName = ResolveQualifiedName(fieldDecl->getQualifiedNameAsString());

		Field* field = new Field(ResolveQualType(fieldDecl->getType()), qualifiedName);

		field->setOffset(m_Context->getFieldOffset(fieldDecl) / 8);

		parent->addField(field);
	}

	QualifiedType* ASTScraper::ResolveQualType(clang::QualType qualType)
	{
		MakeCanonical(qualType);

		QualifiedType* qualifiedType = new QualifiedType();

		qualifiedType->setConst(qualType.isConstant(*m_Context));

		if (auto ptr = clang::dyn_cast<clang::PointerType>(qualType.split().Ty)) {
			qualifiedType->setQualifierOperator(QualifierOperator::POINTER);
			qualType = ptr->getPointeeType();
		}
		else if (auto ref = clang::dyn_cast<clang::ReferenceType>(qualType.split().Ty)) {
			qualifiedType->setQualifierOperator(QualifierOperator::REFERENCE);
			qualType = ptr->getPointeeType();
		} else
			qualifiedType->setQualifierOperator(QualifierOperator::VALUE);

		MakeCanonical(qualType);

		const clang::Type* cType = qualType.split().Ty;

		Type* type = ScrapeType(cType);
		qualifiedType->setTypeID(type ? type->getID() : 0);

		return qualifiedType;
	}

	void ASTScraper::MakeCanonical(clang::QualType& qualType)
	{
		if (!qualType.isCanonical())
			qualType = qualType.getCanonicalType();
	}

	QualifiedName ASTScraper::ResolveQualifiedName(std::string qualifiedName)
	{
		RemoveAll(qualifiedName, "::(anonymous union)");
		RemoveAll(qualifiedName, "::(anonymous struct)");

		return QualifiedName(qualifiedName);
	}

	void ASTScraper::RemoveAll(std::string& source, const std::string& search)
	{
		std::string::size_type n = search.length();
		for (std::string::size_type i = source.find(search); i != std::string::npos; i = source.find(search))
			source.erase(i, n);
	}

	AccessSpecifier ASTScraper::TransformAccess(const clang::AccessSpecifier as)
	{
		switch (as) {
		case clang::AccessSpecifier::AS_public:
		case clang::AccessSpecifier::AS_none:
			return AccessSpecifier::PUBLIC;
		case clang::AccessSpecifier::AS_protected:
			return AccessSpecifier::PROTECTED;
		case clang::AccessSpecifier::AS_private:
			return AccessSpecifier::PRIVATE;
		}
	}

	std::vector<std::string> ASTScraper::ScrapeAnnotations(const clang::Decl* decl)
	{
		std::vector<std::string> annotations;

		if (decl && decl->hasAttrs()) {
			auto it = decl->specific_attr_begin<clang::AnnotateAttr>();
			auto itEnd = decl->specific_attr_end<clang::AnnotateAttr>();

			// __attribute__((annotate("...")))
			for (; it != itEnd; it++) {
				const clang::AnnotateAttr* attr = clang::dyn_cast<clang::AnnotateAttr>(*it);
				if (attr)
					annotations.emplace_back(attr->getAnnotation());
			}
		}

		return annotations;
	}

	bool ASTScraper::IsReflected(const std::vector<std::string>& attrs)
	{
		if (m_Config.AnnotationRequired.size() == 0)
			return true;
		return std::find(attrs.begin(), attrs.end(), m_Config.AnnotationRequired) != attrs.end();
	}

	void ASTScraper::SetContext(clang::ASTContext* context)
	{
		m_Context = context;
	}
}
