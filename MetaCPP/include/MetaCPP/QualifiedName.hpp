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
		QualifiedName(const Namespace& namespace_, const std::string& name, const std::string& templateArgs = "");
		QualifiedName(std::string fullQualified);

		std::string getName() const;
		std::string getTemplateArgs() const;
		std::string getTemplatedName() const;
		Namespace getNamespace() const;
		std::string fullQualified() const;

	private:
		Namespace m_Namespace;
		std::string m_Name, m_TemplateArgs;
	};
}

#endif