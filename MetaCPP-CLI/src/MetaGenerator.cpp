#include "MetaGenerator.hpp"

#include <clang/Frontend/FrontendActions.h>

namespace metacpp {
	MetaGenerator::MetaGenerator(std::vector<std::string> sources, std::vector<std::string> flags) {
		m_CompilationDatabase = new clang::tooling::FixedCompilationDatabase(".", flags);
		m_ClangTool = new clang::tooling::ClangTool(*m_CompilationDatabase, sources);
	}

	MetaGenerator::~MetaGenerator() {
		delete m_ClangTool;
	}

	void MetaGenerator::Generate()
	{
		int result = m_ClangTool->run(clang::tooling::newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
	}
}
