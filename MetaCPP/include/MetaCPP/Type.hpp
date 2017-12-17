#ifndef METACPP_TYPE_HPP
#define METACPP_TYPE_HPP

#include <vector>

#include "TypeID.hpp"
#include "QualifiedName.hpp"
#include "Field.hpp"
#include "Method.hpp"
#include "Exportable.hpp"

namespace metacpp {
	enum TypeKind {
		PRIMITIVE,
		CLASS,
		STRUCT
	};

	enum AccessSpecifier {
		PUBLIC,
		PROTECTED,
		PRIVATE
	};

	struct BaseType {
		QualifiedType* type;
		AccessSpecifier access;
	};

	class Type : public Exportable {
	public:
		Type(const TypeID id, const QualifiedName& qName);

		TypeID getID() const;
		const QualifiedName& getQualifiedName() const;
		TypeKind getKind() const;
		AccessSpecifier getAccess() const;
		size_t getSize() const;
		bool isPrimitive() const;
		bool isStruct() const;
		bool isClass() const;
		bool isValid() const;
		const std::vector<Field*>& getFields() const;
		const std::vector<QualifiedType*> getTemplateArguments() const;
		const std::vector<BaseType>& getBaseTypes() const;

		void setKind(const TypeKind kind);
		void setAccess(const AccessSpecifier access);
		void setSize(const size_t bytes);

		void addBaseType(QualifiedType* baseType, const AccessSpecifier access);
		void addTemplateArgument(QualifiedType* arg, int pos = -1);
		void addField(Field* field);
		void addMethod(Method* method);

		mustache::data asMustache() const override;

	private:
		std::string dumpTemplate() override;

		const TypeID m_ID;
		const QualifiedName m_QualifiedName;
		size_t m_SizeInBytes = -1;
		TypeKind m_Kind = PRIMITIVE;
		AccessSpecifier m_Access = PUBLIC;

		std::vector<BaseType> m_BaseTypes;
		std::vector<QualifiedType*> m_TemplateArguments;
		std::vector<Field*> m_Fields;
		std::vector<Method*> m_Methods;
	};
}

#endif