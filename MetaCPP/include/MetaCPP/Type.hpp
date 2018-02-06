#ifndef METACPP_TYPE_HPP
#define METACPP_TYPE_HPP

#include <vector>

#include "TypeID.hpp"
#include "QualifiedName.hpp"
#include "Field.hpp"
#include "Method.hpp"
#include "Container.hpp"
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
		typedef std::function<void*(void*)> Constructor;

		Type(const TypeID id, const QualifiedName& qName);

		TypeID getID() const;
		const QualifiedName& getQualifiedName() const;
		TypeKind getKind() const;
		AccessSpecifier getAccess() const;
		size_t getSize() const;
		bool isPrimitive() const;
		bool isStruct() const;
		bool isClass() const;
		bool isPolymorphic() const;
		bool isValid() const;
		bool isSTL() const;
		bool isSequentialContainer() const;
		bool isAssociativeContainer() const;
		bool isContainer() const;
		const std::vector<Field*>& getFields() const;
		const std::vector<QualifiedType*> getTemplateArguments() const;
		const std::vector<BaseType>& getBaseTypes() const;
		const std::vector<TypeID>& getDerivedTypes() const;
		Container* getContainer() const;

		void* allocate(void* ptr = 0) const;

		void setKind(const TypeKind kind);
		void setAccess(const AccessSpecifier access);
		void setSize(const size_t bytes);
		void setPolymorphic(const bool polymorphic);
		void setHasDefaultConstructor(const bool hasDefaultConstructor);
		void setConstructor(const Constructor constructor);
		void setContainer(Container* container);

		void addBaseType(QualifiedType* baseType, const AccessSpecifier access);
		void addDerivedType(TypeID typeID);
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
		bool m_Polymorphic;
		bool m_HasDefaultConstructor;
		Container* m_Container;
		Constructor m_Constructor;

		std::vector<BaseType> m_BaseTypes;
		std::vector<TypeID> m_DerivedTypes;
		std::vector<QualifiedType*> m_TemplateArguments;
		std::vector<Field*> m_Fields;
		std::vector<Method*> m_Methods;
	};
}

#endif