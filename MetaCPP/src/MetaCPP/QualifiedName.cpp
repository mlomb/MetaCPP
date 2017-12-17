#include "MetaCPP/QualifiedName.hpp"

#include <sstream>
#include <algorithm>

namespace metacpp {
	QualifiedName::QualifiedName()
		: m_Namespace({}), m_Name(""), m_TemplateArgs("")
	{
	}

	QualifiedName::QualifiedName(const Namespace& namespace_, const std::string& name, const std::string& templateArgs)
		: m_Namespace(namespace_), m_Name(name), m_TemplateArgs(templateArgs)
	{
	}

	QualifiedName::QualifiedName(const Namespace& namespace_)
		: m_Namespace(namespace_), m_Name(""), m_TemplateArgs("") {
		if (!namespace_.empty()) {
			m_Name = namespace_.back();
			m_Namespace.pop_back();
		}

		auto pos = m_Name.find("<");
		if (pos != std::string::npos) {
			m_TemplateArgs = m_Name.substr(pos + 1);
			m_TemplateArgs.pop_back();
			m_Name = m_Name.substr(0, pos);
		}
	}

	QualifiedName::QualifiedName(std::string fullQualified) {
		std::stringstream ss;
		int bracketLevel = 0;
		char c;
		for (size_t pos = 0; pos < fullQualified.size(); pos++) {
			c = fullQualified[pos];
			if (c == ':' && bracketLevel == 0) {
				m_Namespace.push_back(ss.str());
				ss.str("");
				pos++;
			}
			else {
				ss << c;
				if (c == '<') bracketLevel++;
				else if (c == '>') bracketLevel--;
			}
		}
		ss.seekg(0, std::ios::end);
		if ((int)ss.tellg() != 0)
			m_Namespace.push_back(ss.str());

		if (!m_Namespace.empty()) {
			m_Name = m_Namespace.back();
			m_Namespace.pop_back();
		}

		auto pos = m_Name.find("<");
		if (pos != std::string::npos) {
			m_TemplateArgs = m_Name.substr(pos + 1);
			m_TemplateArgs.pop_back();
			m_Name = m_Name.substr(0, pos);
		}
	}

	std::string QualifiedName::fullQualified() const {
		std::stringstream ss;
		for (const std::string& s : m_Namespace)
			ss << s << "::";
		if (!m_Name.empty()) {
			ss << m_Name;
			if (!m_TemplateArgs.empty())
				ss << "<" << m_TemplateArgs << ">";
		}
		else
			ss.seekp(-2, std::ios_base::end);
		return ss.str();
	}

	std::string QualifiedName::getName() const
	{
		return m_Name;
	}
	
	std::string QualifiedName::getTemplateArgs() const
	{
		return m_TemplateArgs;
	}

	std::string QualifiedName::getTemplatedName() const
	{
		return m_Name + "<" + m_TemplateArgs + ">";
	}

	Namespace QualifiedName::getNamespace() const
	{
		return m_Namespace;
	}
}
