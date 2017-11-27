#include <clang/Frontend/FrontendAction.h>

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	class ASTScraperAction : public clang::ASTFrontendAction {
	public:
		ASTScraperAction(Storage* storage);
		
		std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& Compiler, llvm::StringRef InFile);

	private:
		Storage* m_Storage;
	};
}