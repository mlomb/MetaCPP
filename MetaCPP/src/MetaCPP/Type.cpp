#include "MetaCPP/Type.hpp"

#include "MetaCPP/Storage.hpp"
#include "MetaCPP/Templates.hpp"

namespace metacpp {
	Type::Type(const TypeID id, const QualifiedName& qName)
		: m_ID(id),
		m_QualifiedName(qName)
	{
	}

	TypeID Type::getID() const
	{
		return m_ID;
	}

	const QualifiedName& Type::getQualifiedName() const
	{
		return m_QualifiedName;
	}

	TypeKind Type::getKind() const
	{
		return m_Kind;
	}

	AccessSpecifier Type::getAccess() const
	{
		return m_Access;
	}

	size_t Type::getSize() const
	{
		return m_SizeInBytes;
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

	bool Type::isValid() const
	{
		std::string qName = m_QualifiedName.fullQualified();
		return qName.find("INVALID") == std::string::npos
			&& qName.find("UNKNOWN") == std::string::npos;
	}

	const std::vector<Field*>& Type::getFields() const
	{
		return m_Fields;
	}
	
	const std::vector<QualifiedType*> Type::getTemplateArguments() const
	{
		return m_TemplateArguments;
	}

	const std::vector<BaseType>& Type::getBaseTypes() const
	{
		return m_BaseTypes;
	}

	void Type::setKind(const TypeKind kind)
	{
		m_Kind = kind;
	}

	void Type::setAccess(const AccessSpecifier access)
	{
		m_Access = access;
	}

	void Type::setSize(const size_t bytes)
	{
		m_SizeInBytes = bytes;
	}

	void Type::addBaseType(QualifiedType* baseType, const AccessSpecifier access)
	{
		m_BaseTypes.push_back(BaseType{ baseType, access });
	}

	void Type::addTemplateArgument(QualifiedType* arg, int pos)
	{
		if (pos == -1)
			m_TemplateArguments.push_back(arg);
		else
			m_TemplateArguments.insert(m_TemplateArguments.begin() + pos, arg);
	}

	void Type::addField(Field* field)
	{
		field->setOwner(m_ID);
		m_Fields.emplace_back(field);
	}

	void Type::addMethod(Method* method)
	{
		//method->setOwner(m_ID);
		m_Methods.emplace_back(method);
	}

	mustache::data Type::asMustache() const
	{
		mustache::data data;
		data["id"] = std::to_string(m_ID);
		data["qualifiedName"] = m_QualifiedName.fullQualified();
		data["name"] = m_QualifiedName.getName();
		data["size"] = std::to_string(m_SizeInBytes);
		data["kind"] = std::to_string(m_Kind);
		data["access"] = std::to_string(m_Access);
		data["valid"] = std::to_string(isValid());

		// Base Types
		mustache::data baseTypes{ mustache::data::type::list };
		for (const BaseType& base : m_BaseTypes) {
			mustache::data baseType;
			baseType["qualifiedType"] = base.type->asMustache();
			baseType["access"] = std::to_string(base.access);
			baseTypes << baseType;
		}

		// Template Arguments
		mustache::data templateArguments{ mustache::data::type::list };
		for (const QualifiedType* arg : m_TemplateArguments) {
			if (arg)
				templateArguments << arg->asMustache();
			else
				templateArguments << mustache::data{ "null", false };
		}

		// Fields
		mustache::data fields{ mustache::data::type::list };
		for (const Field* field : m_Fields)
			fields << field->asMustache();


		data["fields"] = mustache::data{ fields };
		data["baseTypes"] = mustache::data{ baseTypes };
		data["templateArguments"] = mustache::data{ templateArguments };

		return data;
	}

	std::string Type::dumpTemplate() 
	{
		return templates::dump_type;
	}
}
