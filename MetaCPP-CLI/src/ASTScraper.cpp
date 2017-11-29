#include "ASTScraper.hpp"

#include <iostream>

#include <clang/AST/RecordLayout.h>
#include <clang/AST/DeclTemplate.h>
#include "clang/AST/Decl.h"

#include "MetaCPP/Type.hpp"

namespace metacpp {
	ASTScraper::ASTScraper(clang::ASTContext* context, Storage* storage)
		: m_Context(context), m_Storage(storage)
	{
	}
		
	void ASTScraper::ScrapeDecl(const clang::Decl* decl)
	{
		/* Types */
		const clang::TypeDecl* typeDecl = clang::dyn_cast<clang::TypeDecl>(decl);
		if (typeDecl)
			ScrapeTypeDecl(typeDecl);

		/* Enums */
		// TODO
	}

	TypeID ASTScraper::ScrapeTypeDecl(const clang::TypeDecl* typeDecl)
	{
		if (!typeDecl)
			return 0;

		
		auto typedefDecl = clang::dyn_cast<clang::TypedefDecl>(typeDecl);
		if (typedefDecl) {
			TypeID id = AddType(typedefDecl->getUnderlyingType().getTypePtr());
			if (id)
				m_Storage->assignTypeID(typedefDecl->getQualifiedNameAsString(), id);
			return id;
		}

		const clang::Type* type = typeDecl->getTypeForDecl();
		if (type)
			return AddType(type);

		return 0;
	}

	TypeID ASTScraper::AddType(const clang::Type* c_type)
	{
		// We can only scrape complete types
		// TODO Investigate why
		//		typedef A<T> B;
		// if A<T> is not used elsewhere
		// is considered an incomplete type
		if (!c_type || c_type->isIncompleteType())
			return 0;
		
		// If it its an elaborated type, we scrape the underlying type
		auto elType = clang::dyn_cast<clang::ElaboratedType>(c_type);
		if (elType)
			return AddType(elType->getNamedType().getTypePtr());

		// If it is a typedef
		auto typedefType = clang::dyn_cast<clang::TypedefType>(c_type);
		if (typedefType)
			return ScrapeTypeDecl(typedefType->getDecl());
		
		// <fullName, name>
		auto& names = GetNameFromType(c_type);

		if (names.first.size() == 0)
			return 0;

		TypeID typeId = m_Storage->assignTypeID(names.first);
		
		// Have we already scraped this type?
		if (m_Storage->hasType(typeId))
			return typeId;
		
		Type* type = new Type(typeId, names.first, names.second);

		type->setSize(m_Context->getTypeSize(c_type) / 8);

		auto rDecl = c_type->getAsCXXRecordDecl();
		if (rDecl) {
			ScrapeRecord(rDecl, type);
		}
		else {
			type->setKind(TypeKind::PRIMITIVE);
		}

		m_Storage->addType(type);

		return typeId;
	}

	std::pair<std::string, std::string> ASTScraper::GetNameFromType(const clang::Type* type)
	{
		auto rDecl = type->getAsCXXRecordDecl();
		auto binType = type->getAs<clang::BuiltinType>();
		auto tsType = clang::dyn_cast<clang::TemplateSpecializationType>(type);

		if (type->isBuiltinType()) {
			if (binType) {
				static clang::LangOptions lang_opts;
				static clang::PrintingPolicy printing_policy(lang_opts);

				std::string name = binType->getName(printing_policy);
				return std::make_pair(name, name);
			}
		}
		else if (rDecl) {
			std::string fullName = rDecl->getQualifiedNameAsString();
			std::string name = rDecl->getNameAsString();

			if (tsType) {
				std::string specialized_args = "<";
				clang::ArrayRef<clang::TemplateArgument> args = tsType->template_arguments();
				for (const clang::TemplateArgument& arg : args) {
					TypeID arg_hash = AddType(arg.getAsType().getTypePtr());
					if (arg_hash)
						specialized_args += m_Storage->getType(arg_hash)->getFullName() + ",";
				}
				if (tsType->getNumArgs() > 0)
					specialized_args.pop_back();
				specialized_args += ">";

				return std::make_pair(fullName + specialized_args, name + specialized_args);
			}
			else if (rDecl->hasDefinition() && !rDecl->isImplicit() && !rDecl->isDependentType())
				return std::make_pair(fullName, name);
		}


		return std::make_pair("", "");
	}

	void ASTScraper::ScrapeRecord(const clang::CXXRecordDecl* rDecl, Type* type)
	{
		type->setKind(rDecl->isStruct() ? TypeKind::STRUCT : TypeKind::CLASS);

		/* Fields */
		for (auto it = rDecl->field_begin(); it != rDecl->field_end(); it++) {
			clang::FieldDecl* fieldDecl = *it;
			if (fieldDecl) {
				FieldID fieldId = ScrapeField(fieldDecl, type);
				if(fieldId != 0)
					type->addField(fieldId);
			}
		}
	}

	FieldID ASTScraper::ScrapeField(const clang::FieldDecl* fDecl, const Type* owner)
	{
		clang::QualType qualType = fDecl->getType();
		const clang::Type* c_type = qualType.getTypePtr();

		TypeID typeId = AddType(c_type);
		if (!typeId)
			return 0; // NULL

		std::string name = fDecl->getNameAsString();
		std::string fullName = owner->getFullName() + "::" + name;
		FieldID fieldId = m_Storage->assignFieldID(fullName);

		Field* field = new Field(fieldId, typeId, owner->getID(), fullName, name);

		unsigned int offset_bytes = m_Context->getFieldOffset(fDecl) / 8;
		field->setOffset(offset_bytes);

		m_Storage->addField(field);

		return fieldId;
	}
}
