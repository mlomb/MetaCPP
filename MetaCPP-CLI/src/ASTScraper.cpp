#include "ASTScraper.hpp"

#include <iostream>

#include <clang/AST/RecordLayout.h>

#include "MetaCPP/Type.hpp"

namespace metacpp {
	ASTScraper::ASTScraper(clang::ASTContext* context, Storage* storage)
		: m_Context(context), m_Storage(storage)
	{
	}
		
	void ASTScraper::ScrapeDecl(const clang::Decl* decl)
	{
		decl->dump();
		/* Types */
		const clang::TypeDecl* typeDecl = clang::dyn_cast<clang::TypeDecl>(decl);
		if (typeDecl) {
			const clang::Type* type = typeDecl->getTypeForDecl();
			if (type)
				AddType(type);
		}

		/* Enums */
		// TODO
	}

	TypeHash ASTScraper::AddType(const clang::Type* c_type)
	{
		const std::string name = GetNameFromType(c_type);

		if (name.size() == 0)
			return 0;

		const TypeHash type_hash = HashFromString(name);

		if (m_Storage->HasType(type_hash))
			return type_hash;
		
		Type* type = new Type(type_hash);

		type->name = name;
		type->size_bytes = m_Context->getTypeSize(c_type) / 8;

		auto rDecl = c_type->getAsCXXRecordDecl();
		if (rDecl)
			ScrapeRecord(type, rDecl);
		else { // primitive
			type->is_primitive = true;

		}

		m_Storage->AddType(type);

		return type_hash;
	}

	std::string ASTScraper::GetNameFromType(const clang::Type* type)
	{
		auto rDecl = type->getAsCXXRecordDecl();

		if (rDecl) {
			if (rDecl->hasDefinition() && !rDecl->isImplicit() && !rDecl->isDependentType())
				return rDecl->getName();
		}
		else if (type->isBuiltinType()) {
			auto binType = type->getAs<clang::BuiltinType>();
			if (binType) {
				static clang::LangOptions lang_opts;
				static clang::PrintingPolicy printing_policy(lang_opts);

				return binType->getName(printing_policy);
			}
		}

		return "";
	}

	void ASTScraper::ScrapeRecord(Type* type, const clang::CXXRecordDecl* rDecl)
	{
		type->is_primitive = false;
		type->is_struct = rDecl->isStruct();
		/* Fields information */
		for (auto it = rDecl->field_begin(); it != rDecl->field_end(); it++) {
			clang::FieldDecl* fieldDecl = *it;
			if (fieldDecl) {
				Field* field = ScrapeField(fieldDecl);
				if (field) {
					field->owner_type = type->id;
					type->fields.push_back(field);
				}
			}
		}
	}

	Field* ASTScraper::ScrapeField(const clang::FieldDecl* fDecl)
	{
		clang::QualType field_qual_type = fDecl->getType();
		const clang::Type* c_type = field_qual_type.getTypePtr();

		TypeHash type_hash = AddType(c_type);
		if (!type_hash)
			return 0; // NULL

		Field* field = new Field();

		field->type = type_hash;
		field->name = fDecl->getNameAsString();

		return field;
	}
}
