#include "ASTScraperVisitor.hpp"
#include <iostream>
namespace metacpp {
	ASTScraperVisitor::ASTScraperVisitor(clang::ASTContext* context, Storage* storage)
		: m_Scraper(context, storage)
	{
	}

	bool ASTScraperVisitor::TraverseDecl(clang::Decl* D)
	{
		if (!D)
			return true;

		m_Scraper.ScrapeDecl(D);

		return clang::RecursiveASTVisitor<ASTScraperVisitor>::TraverseDecl(D);
	}
}