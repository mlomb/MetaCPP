#include "MetaCPP/Method.hpp"

namespace metacpp {
	MethodParameter::MethodParameter(const std::string& name, const QualifiedType& type)
			: m_Name(name), m_Type(type) {
	}

	const std::string& MethodParameter::GetName() const {
		return m_Name;
	}

	const QualifiedType& MethodParameter::GetType() const {
		return m_Type;
	}

	Method::Method(const QualifiedName& qName)
			: m_QualifiedName(qName), m_Owner(0) {
	}

	TypeID Method::GetOwnerType() const {
		return m_Owner;
	}

	const QualifiedName& Method::GetQualifiedName() const {
		return m_QualifiedName;
	}

	void Method::SetOwner(const TypeID ownerTypeId) {
		m_Owner = ownerTypeId;
	}

	void Method::AddParameter(const MethodParameter& parameter) {
		m_Parameters.push_back(parameter);
	}
}
