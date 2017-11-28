#include <clang/AST/DeclCXX.h>

#include "MetaCPP/Storage.hpp"

namespace metacpp {

	class ASTScraper {
	public:
		ASTScraper(clang::ASTContext* context, Storage* storage);

		std::pair<std::string, std::string> GetNameFromType(const clang::Type* type);

		TypeID AddType(const clang::Type* type);

		void ScrapeDecl(const clang::Decl* decl);
		void ScrapeRecord(const clang::CXXRecordDecl* rDecl, Type* type);
		FieldID ScrapeField(const clang::FieldDecl* fDecl, const Type* owner);

	private:
		clang::ASTContext* m_Context;
		Storage* m_Storage;
	};
}