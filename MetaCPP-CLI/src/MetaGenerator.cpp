#include "MetaGenerator.hpp"

#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>

#include "ASTScraperAction.hpp"

namespace metacpp {
	MetaGenerator::MetaGenerator(std::vector<std::string> sources, std::vector<std::string> flags) {
		m_CompilationDatabase = new clang::tooling::FixedCompilationDatabase(".", flags);
		m_ClangTool = new clang::tooling::ClangTool(*m_CompilationDatabase, sources);
	}

	MetaGenerator::~MetaGenerator() {
		delete m_ClangTool;
	}

	void MetaGenerator::Generate(Storage* storage)
	{
		class ActionFactory : public clang::tooling::FrontendActionFactory {
		public:
			ActionFactory(Storage* storage) : storage(storage) {};
			clang::FrontendAction *create() override { return new ASTScraperAction(storage); }
		private:
			Storage* storage;
		};
		auto scraperAction = std::unique_ptr<ActionFactory>(new ActionFactory(storage));

		int result = m_ClangTool->run(scraperAction.get());
	}
}
