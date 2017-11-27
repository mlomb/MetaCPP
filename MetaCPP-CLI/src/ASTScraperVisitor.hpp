#include <clang/AST/RecursiveASTVisitor.h>

#include "ASTScraper.hpp"

namespace metacpp {

	class ASTScraperVisitor : public clang::RecursiveASTVisitor<ASTScraperVisitor> {
	public:
		ASTScraperVisitor(clang::ASTContext* context, Storage* storage);

		bool TraverseDecl(clang::Decl* D);

	private:
		ASTScraper m_Scraper;
	};
}