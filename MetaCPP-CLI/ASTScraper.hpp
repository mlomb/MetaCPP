#ifndef METACPP_ASTSCRAPER_HPP
#define METACPP_ASTSCRAPER_HPP

#include <clang/AST/DeclCXX.h>

#include "MetaCPP/Storage.hpp"
#include "MetaCPP/QualifiedName.hpp"
#include "MetaCPP/QualifiedType.hpp"

namespace metacpp {
	class ASTScraper {
	public:
		struct Configuration {
			std::string AnnotationRequired;
		};

		ASTScraper(Storage* storage, const Configuration& config);

		void ScrapeTranslationUnit(const clang::TranslationUnitDecl* tuDecl);
		void ScrapeDeclContext(const clang::DeclContext* ctxDecl, Type* parent);
		void ScrapeNamedDecl(const clang::NamedDecl* namedDecl, Type* parent);

		Type* ScrapeCXXRecordDecl(const clang::CXXRecordDecl* cxxRecordDecl, Type* parent);
		std::vector<QualifiedType> ResolveCXXRecordTemplate(const clang::CXXRecordDecl* cxxRecordDecl, QualifiedName& qualifiedName);
		Type* ScrapeType(const clang::Type* cType);
		void ScrapeFieldDecl(const clang::FieldDecl* fieldDecl, Type* parent);
		void ScrapeMethodDecl(const clang::CXXMethodDecl* cxxMethodDecl, Type* parent);

		QualifiedType ResolveQualType(clang::QualType qualType);
		std::vector<std::string> ScrapeAnnotations(const clang::Decl* decl);

		/* Utility */
		void MakeCanonical(clang::QualType& qualType);
		QualifiedName ResolveQualifiedName(std::string qualifiedName);
		void RemoveAll(std::string& source, const std::string& search);
		AccessSpecifier TransformAccess(const clang::AccessSpecifier as);

		bool IsReflected(const std::vector<std::string>& attrs);

		void SetContext(clang::ASTContext* context);

	private:
		clang::ASTContext* m_Context;
		Configuration m_Config;
		Storage* m_Storage;
	};
}

#endif