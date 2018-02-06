#include "MetaCPP/Method.hpp"

namespace metacpp {
	MethodParameter::MethodParameter(const std::string& name, const QualifiedType& type)
		: m_Name(name), m_Type(type)
	{
	}

	Method::Method(const QualifiedName& qName)
		: m_QualifiedName(qName)
	{
	}

	TypeID Method::getOwnerType() const {
		return m_Owner;
	}

	const QualifiedName& Method::getQualifiedName() const
	{
		return m_QualifiedName;
	}

	void Method::setOwner(const TypeID ownerTypeId)
	{
		m_Owner = ownerTypeId;
	}

	void Method::addParameter(const MethodParameter& parameter)
	{
		m_Parameters.push_back(parameter);
	}
}
