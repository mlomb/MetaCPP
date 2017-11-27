#include "ASTScraperConsumer.hpp"

namespace metacpp {
	ASTScraperConsumer::ASTScraperConsumer(clang::ASTContext* context, Storage* storage)
		: m_Visitor(context, storage)
	{
	}

	void ASTScraperConsumer::HandleTranslationUnit(clang::ASTContext& context)
	{
		m_Visitor.TraverseDecl(context.getTranslationUnitDecl());
	}
}