#ifndef METACPP_SCRAPERTOOL_HPP
#define METACPP_SCRAPERTOOL_HPP

#include <vector>
#include <string>

#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>

namespace metacpp {
	class ASTScraper;

	class ScraperTool {
	public:
		ScraperTool(std::string source, std::vector<std::string> flags);
		~ScraperTool();

		void Run(ASTScraper *scraper);

	private:
		clang::tooling::FixedCompilationDatabase *m_CompilationDatabase;
		clang::tooling::ClangTool *m_ClangTool;
	};
}

#endif