#include <clang/AST/DeclCXX.h>

#include "MetaCPP/Storage.hpp"

namespace metacpp {

	class ASTScraper {
	public:
		ASTScraper(clang::ASTContext* context, Storage* storage);

		void ScrapeRecord(clang::CXXRecordDecl* rDecl);
		
	private:
		clang::ASTContext* m_Context;
		Storage* m_Storage;
	};
}