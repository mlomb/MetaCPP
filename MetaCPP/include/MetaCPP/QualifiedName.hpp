#ifndef METACPP_QUALIFIEDNAME_HPP
#define METACPP_QUALIFIEDNAME_HPP

#include <vector>
#include <string>

namespace metacpp {
	typedef std::vector<std::string> Namespace;

	class QualifiedName {
	public:
		QualifiedName();
		QualifiedName(const Namespace& namespace_);
		QualifiedName(const Namespace& namespace_, const std::string& name, const std::string& templateArgs, const std::string& arraySize);
		QualifiedName(std::string fullQualified);

		std::string GetName() const;
		std::string GetTemplateArgs() const;
		std::string GetTemplatedName() const;
		std::string FullQualified() const;
		std::string MemberQualified(const std::string& name) const;
		const Namespace& GetNamespace() const;

	private:
		Namespace m_Namespace;
		std::string m_Name, m_TemplateArgs, m_ArraySize;
	};
}

#endif