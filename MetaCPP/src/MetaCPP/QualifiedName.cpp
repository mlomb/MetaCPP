#include "MetaCPP/QualifiedName.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

namespace metacpp {
	QualifiedName::QualifiedName()
			: m_Namespace({}), m_Name(""), m_TemplateArgs(""), m_ArraySize("") {
	}

	QualifiedName::QualifiedName(const Namespace& namespace_, const std::string& name, const std::string& templateArgs, const std::string& arraySize)
			: m_Namespace(namespace_), m_Name(name), m_TemplateArgs(templateArgs), m_ArraySize(arraySize) {
	}

	QualifiedName::QualifiedName(const Namespace& namespace_)
			: m_Namespace(namespace_), m_Name(""), m_TemplateArgs(""), m_ArraySize("") {
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
			} else if (c == '[' && bracketLevel == 0) {
				ss.seekg(0, std::ios::end);
				if ((int) ss.tellg() != 0)
					m_Namespace.push_back(ss.str());
				ss.str("");
			} else if (c == ']' && bracketLevel == 0) {
				ss.seekg(0, std::ios::end);
				if ((int) ss.tellg() != 0) {
					m_ArraySize = ss.str();
					// std::cout << "ArraySize read: " << m_ArraySize << " for " << fullQualified << std::endl;
				}
				ss.str("");
			} else {
				ss << c;
				if (c == '<') bracketLevel++;
				else if (c == '>') bracketLevel--;
			}
		}
		ss.seekg(0, std::ios::end);
		if ((int) ss.tellg() != 0)
			m_Namespace.push_back(ss.str());

		if (!m_Namespace.empty()) {
			m_Name = m_Namespace.back();
			m_Namespace.pop_back();
		}

		if (m_Name.substr(0, 8) != "operator") {
			auto pos = m_Name.find("<");
			if (pos != std::string::npos) {
				m_TemplateArgs = m_Name.substr(pos + 1);
				m_TemplateArgs.pop_back();
				m_Name = m_Name.substr(0, pos);
			}
		}
	}

	std::string QualifiedName::FullQualified() const {
		std::stringstream ss;
		for (const std::string& s : m_Namespace)
			ss << s << "::";
		if (!m_Name.empty()) {
			ss << m_Name;
			if (!m_TemplateArgs.empty())
				ss << "<" << m_TemplateArgs << ">";
			if (!m_ArraySize.empty())
				ss << "[" << m_ArraySize << "]";
		} else
			ss.seekp(-2, std::ios_base::end);
		return ss.str();
	}

	std::string QualifiedName::GetName() const {
		return m_Name;
	}

	std::string QualifiedName::GetTemplateArgs() const {
		return m_TemplateArgs;
	}

	std::string QualifiedName::GetTemplatedName() const {
		return m_Name + (m_TemplateArgs.size() > 0 ? "<" + m_TemplateArgs + ">" : "");
	}

	const Namespace& QualifiedName::GetNamespace() const {
		return m_Namespace;
	}

	std::string QualifiedName::MemberQualified(const std::string& name) const {
		std::stringstream ss;
		for (const std::string& s : m_Namespace)
			ss << s << "::";
		if (!m_Name.empty()) {
			ss << m_Name;
			if (!m_TemplateArgs.empty())
				ss << "<" << m_TemplateArgs << ">";
			ss << " " << name;
			if (!m_ArraySize.empty())
				ss << "[" << m_ArraySize << "]";
		} else
			ss.seekp(-2, std::ios_base::end);
		return ss.str();
	}

	std::string QualifiedName::ElementTypeQualified() const {
		std::stringstream ss;
		for (const std::string& s : m_Namespace)
			ss << s << "::";
		if (!m_Name.empty()) {
			ss << m_Name;
			if (!m_TemplateArgs.empty())
				ss << "<" << m_TemplateArgs << ">";
		} else
			ss.seekp(-2, std::ios_base::end);
		return ss.str();
	}
}
