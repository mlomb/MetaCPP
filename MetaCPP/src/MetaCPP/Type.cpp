#include "MetaCPP/Type.hpp"

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	Type::Type(const TypeID id, const std::string& fullName, const std::string& name)
		: m_ID(id),
		m_FullName(fullName),
		m_Name(name)
	{
	}

	void Type::dump_obj(std::ostream& o)
	{
		o << indentation << m_FullName << std::endl;
		indent();
		{
			o << indentation << "id=" << m_ID << std::endl;
			o << indentation << "size=" << m_SizeInBytes << " bytes" << std::endl;

			if (m_Fields.size() > 0) {
				o << indentation << "Fields:" << std::endl;
				indent();
				{
					for (const FieldID& fieldId : m_Fields)
						dump_storage->getField(fieldId)->dump();
				}
				deindent();
			}
		}
		deindent();
	}

	TypeID Type::getID() const
	{
		return m_ID;
	}

	std::string Type::getName() const
	{
		return m_Name;
	}

	std::string Type::getFullName() const
	{
		return m_FullName;
	}

	TypeKind Type::getKind() const
	{
		return m_Kind;
	}

	bool Type::isPrimitive() const
	{
		return m_Kind == TypeKind::PRIMITIVE;
	}

	bool Type::isStruct() const
	{
		return m_Kind == TypeKind::STRUCT;
	}

	bool Type::isClass() const
	{
		return m_Kind == TypeKind::CLASS;
	}

	void Type::setKind(const TypeKind kind)
	{
		m_Kind = kind;
	}

	void Type::setSize(const unsigned int bytes)
	{
		m_SizeInBytes = bytes;
	}

	void Type::addField(const FieldID field)
	{
		m_Fields.emplace_back(field);
	}

	void Type::addMethod(const MethodID method)
	{
		m_Methods.emplace_back(method);
	}
}
