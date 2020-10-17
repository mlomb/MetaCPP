#ifndef METACPP_TYPE_HPP
#define METACPP_TYPE_HPP

#include <vector>
#include <functional>
#include <cstring>
#include <algorithm>

#include "TypeID.hpp"
#include "QualifiedName.hpp"
#include "Field.hpp"
#include "Method.hpp"
#include "Container.hpp"

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
		QualifiedType type;
		AccessSpecifier access;
	};

	class Type {
	public:
        typedef std::function<void*(void*)> Constructor;
        typedef std::function<void(void*)> Destructor;

		Type(const TypeID id, const QualifiedName& qName);

		TypeID GetTypeID() const;
		const QualifiedName& GetQualifiedName() const;
		TypeKind GetKind() const;
		AccessSpecifier GetAccess() const;
        size_t GetSize() const;
        bool HasSize() const;
		bool IsPrimitive() const;
		bool IsStruct() const;
		bool IsClass() const;
		bool IsPolymorphic() const;
		bool IsValid() const;
		bool IsSTL() const;
        bool IsSequentialContainer() const;
        bool IsStaticArray() const;
		bool IsAssociativeContainer() const;
		bool IsContainer() const;
		const std::vector<Field>& GetFields() const;
		const std::vector<TemplateArgument> GetTemplateArguments() const;
		const std::vector<BaseType>& GetBaseTypes() const;
		const std::vector<TypeID>& GetDerivedTypes() const;
		Container* GetContainer() const;

        void* Allocate(void* ptr = 0) const;
        void Delete(void* ptr = 0) const;

		void SetKind(const TypeKind kind);
		void SetAccess(const AccessSpecifier access);
		void SetSize(const size_t bytes);
		void SetPolymorphic(const bool polymorphic);
        void SetHasDefaultConstructor(const bool hasDefaultConstructor);
        void SetHasDefaultDestructor(const bool hasDefaultDestructor);
        void SetConstructor(const Constructor constructor);
        void SetDestructor(const Destructor destructor);
		void SetContainer(Container* container);

		void AddBaseType(const QualifiedType& baseType, const AccessSpecifier access);
		void AddDerivedType(const TypeID typeID);
		void AddTemplateArgument(const TemplateArgument& arg, const int pos = -1);
		void AddField(Field& field);
		void AddMethod(Method& method);
		
	private:
		const TypeID m_ID;
		const QualifiedName m_QualifiedName;
		size_t m_SizeInBytes;
		TypeKind m_Kind;
		AccessSpecifier m_Access;
		bool m_Polymorphic;
		bool m_HasDefaultConstructor;
        bool m_HasDefaultDestructor;
		Container* m_Container;
        Constructor m_Constructor;
        Destructor m_Destructor;

		std::vector<BaseType> m_BaseTypes;
		std::vector<TypeID> m_DerivedTypes;
		std::vector<TemplateArgument> m_TemplateArguments;
		std::vector<Field> m_Fields;
		std::vector<Method> m_Methods;

		friend class MetaExporter;
	};
}

#endif