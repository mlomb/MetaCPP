#include "ASTScraperVisitor.hpp"

namespace metacpp {
	ASTScraperVisitor::ASTScraperVisitor(clang::ASTContext* context, Storage* storage)
		: m_Scraper(context, storage)
	{
	}

	bool ASTScraperVisitor::TraverseDecl(clang::Decl* D)
	{
		if (!D)
			return true;

		clang::CXXRecordDecl* rDecl = clang::dyn_cast<clang::CXXRecordDecl>(D);
		if (rDecl) m_Scraper.ScrapeRecord(rDecl);

		return clang::RecursiveASTVisitor<ASTScraperVisitor>::TraverseDecl(D);
	}
}