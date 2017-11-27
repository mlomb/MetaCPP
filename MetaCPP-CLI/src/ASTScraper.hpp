#include <clang/AST/DeclCXX.h>

#include "MetaCPP/Storage.hpp"

namespace metacpp {

	class ASTScraper {
	public:
		ASTScraper(clang::ASTContext* context, Storage* storage);

		std::string GetNameFromType(const clang::Type* type);

		TypeHash AddType(const clang::Type* type);

		void ScrapeDecl(const clang::Decl* decl);
		void ScrapeRecord(Type* type, const clang::CXXRecordDecl* rDecl);
		Field* ScrapeField(const clang::FieldDecl* fDecl);
		
	private:
		clang::ASTContext* m_Context;
		Storage* m_Storage;
	};
}