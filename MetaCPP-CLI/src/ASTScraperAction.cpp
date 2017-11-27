#include "ASTScraperAction.hpp"

#include <clang/Frontend/CompilerInstance.h>

#include "ASTScraperConsumer.hpp"

namespace metacpp {
	ASTScraperAction::ASTScraperAction(Storage* storage)
		: m_Storage(storage)
	{
	}

	std::unique_ptr<clang::ASTConsumer> ASTScraperAction::CreateASTConsumer(clang::CompilerInstance& Compiler, llvm::StringRef InFile)
	{
		return std::unique_ptr<clang::ASTConsumer>(new ASTScraperConsumer(&Compiler.getASTContext(), m_Storage));
	}
}