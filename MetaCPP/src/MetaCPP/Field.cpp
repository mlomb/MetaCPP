#include "MetaCPP/Field.hpp"

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	Field::Field(QualifiedType* type, const QualifiedName& qName)
		: m_Type(type),
		m_QualifiedName(qName)
	{
	}

	mustache::data Field::asMustache() const {
		mustache::data data;
		data["ownerId"] = std::to_string(m_Owner);
		data["qualifiedType"] = m_Type->asMustache();
		data["qualifiedName"] = m_QualifiedName.fullQualified();
		data["name"] = m_QualifiedName.getName();
		data["offset"] = std::to_string(m_OffsetInBytes);
		return data;
	}
	
	QualifiedType* Field::getType() const
	{
		return m_Type;
	}

	TypeID Field::getOwnerType() const
	{
		return m_Owner;
	}

	const QualifiedName& Field::getQualifiedName() const
	{
		return m_QualifiedName;
	}
	
	void Field::setOwner(const TypeID ownerTypeId)
	{
		m_Owner = ownerTypeId;
	}

	void Field::setOffset(const unsigned int bytes)
	{
		m_OffsetInBytes = bytes;
	}
}
