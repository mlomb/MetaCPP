#include "MetaCPP/Field.hpp"

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	Field::Field(const FieldID id, const TypeID type, const TypeID owner, const std::string fullName, const std::string name)
		: m_ID(id),
		m_Type(type),
		m_Owner(owner),
		m_FullName(fullName),
		m_Name(name)
	{
	}

	void Field::dump_obj(std::ostream& o)
	{
		std::string type_name = dump_storage->getType(m_Type)->getFullName();
		o << indentation << m_Name << " : " << type_name << std::endl;
		indent();
		{
			o << indentation << "id=" << m_ID << std::endl;
			o << indentation << "offset=" << m_OffsetInBytes << " bytes" << std::endl;

		}
		deindent();
	}

	FieldID Field::getID() const
	{
		return m_ID;
	}

	TypeID Field::getType() const
	{
		return m_Type;
	}

	TypeID Field::getOwnerType() const
	{
		return m_Owner;
	}

	std::string Field::getName() const
	{
		return m_Name;
	}

	std::string Field::getFullName() const
	{
		return m_FullName;
	}

	void Field::setType(const TypeID typeId)
	{
		m_Type = typeId;
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
