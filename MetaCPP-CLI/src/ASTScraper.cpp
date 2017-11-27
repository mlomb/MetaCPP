#include "ASTScraper.hpp"

#include <iostream>

#include "MetaCPP/Type.hpp"

namespace metacpp {
	ASTScraper::ASTScraper(clang::ASTContext* context, Storage* storage)
		: m_Context(context), m_Storage(storage)
	{
	}

	void ASTScraper::ScrapeRecord(clang::CXXRecordDecl* rDecl)
	{
		const TypeHash hash = HashFromString(rDecl->getName().str());
		if (!m_Storage->HasType(hash)) {
			Type* type = new Type(hash);

			type->name = rDecl->getNameAsString();

			m_Storage->AddType(type);
		}
	}
}