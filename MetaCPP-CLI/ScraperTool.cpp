#include "ScraperTool.hpp"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>

#include "ASTScraper.hpp"

namespace metacpp {
	ScraperTool::ScraperTool(std::string source, std::vector<std::string> flags)
	{
#if _DEBUG
		// For now
		flags.push_back("-D_DEBUG");
#endif

		m_CompilationDatabase = new clang::tooling::FixedCompilationDatabase(".", flags);
		m_ClangTool = new clang::tooling::ClangTool(*m_CompilationDatabase, source);
	}

	ScraperTool::~ScraperTool() {
		delete m_CompilationDatabase;
		delete m_ClangTool;
	}

	void ScraperTool::Run(ASTScraper* scraper)
	{
		// Consumer
		class ASTScraperConsumer : public clang::ASTConsumer {
		public:
			ASTScraperConsumer(ASTScraper* scraper) : scraper(scraper) {};

			void HandleTranslationUnit(clang::ASTContext& context) {
				scraper->ScrapeTranslationUnit(context.getTranslationUnitDecl());
			}
		private:
			ASTScraper* scraper;
		};

		// Action
		class ASTScraperAction : public clang::ASTFrontendAction {
		public:
			ASTScraperAction(ASTScraper* scraper) : scraper(scraper) {};
			std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& Compiler, llvm::StringRef InFile) override {
				scraper->SetContext(&Compiler.getASTContext());
				return std::unique_ptr<clang::ASTConsumer>(new ASTScraperConsumer(scraper));
			};
		private:
			ASTScraper* scraper;
		};

		// Factory
		class ActionFactory : public clang::tooling::FrontendActionFactory {
		public:
			ActionFactory(ASTScraper* scraper) : scraper(scraper) {};
			clang::FrontendAction *create() override { return new ASTScraperAction(scraper); }
		private:
			ASTScraper* scraper;
		};
		auto scraperAction = std::unique_ptr<ActionFactory>(new ActionFactory(scraper));

		m_ClangTool->run(scraperAction.get());
	}
}
