#include "MetaCPP/QualifiedType.hpp"

#include "MetaCPP/Storage.hpp"
#include "MetaCPP/Type.hpp"

namespace metacpp {
	QualifiedType::QualifiedType()
	{
	}

	std::string QualifiedType::getQualifiedName(const Storage* storage) const
	{
		Type* type = storage->getType(m_Type);

		std::string result = "";
		if (m_Const)
			result += "const ";
		if (type)
			result += type->getQualifiedName().fullQualified();
		else
			result += "UNKNOWN";

		switch (m_Operator)
		{
		case QualifierOperator::POINTER:
			result += "*";
			break;
		case QualifierOperator::REFERENCE:
			result += "&";
			break;
		}

		return result;
	}

	mustache::data QualifiedType::asMustache() const
	{
		mustache::data data;
		data["typeID"] = std::to_string(m_Type);
		data["const"] = std::to_string(m_Const);
		data["operator"] = std::to_string(m_Operator);
		return data;
	}

	std::string QualifiedType::dumpTemplate()
	{
		return std::string();
	}

	void QualifiedType::setTypeID(const TypeID typeID)
	{
		m_Type = typeID;
	}

	void QualifiedType::setQualifierOperator(const QualifierOperator qualifierOperator)
	{
		m_Operator = qualifierOperator;
	}

	void QualifiedType::setConst(const bool is_const)
	{
		m_Const = is_const;
	}

	TypeID QualifiedType::getTypeID() const
	{
		return m_Type;
	}

	QualifierOperator QualifiedType::getQualifierOperator() const
	{
		return m_Operator;
	}

	bool QualifiedType::isConst() const
	{
		return m_Const;
	}
}
