#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

#include "ASTScraperVisitor.hpp"

namespace metacpp {
	class ASTScraperConsumer : public clang::ASTConsumer {
	public:
		ASTScraperConsumer(clang::ASTContext* context, Storage* storage);

		void HandleTranslationUnit(clang::ASTContext& context);
	private:
		ASTScraperVisitor m_Visitor;
	};
}