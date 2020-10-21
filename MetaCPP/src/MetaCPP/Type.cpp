#include "MetaCPP/Type.hpp"

#include "MetaCPP/Storage.hpp"
#include "MetaCPP/QualifiedType.hpp"

namespace metacpp {
	Type::Type(const TypeID id, const QualifiedName& qName)
			: m_ID(id),
			  m_QualifiedName(qName),
			  m_SizeInBytes(-1),
			  m_Kind(TypeKind::PRIMITIVE),
			  m_Access(AccessSpecifier::PUBLIC),
			  m_Polymorphic(false),
			  m_HasDefaultConstructor(false),
			  m_Container(0) {
	}

	TypeID Type::GetTypeID() const {
		return m_ID;
	}

	const QualifiedName& Type::GetQualifiedName() const {
		return m_QualifiedName;
	}

	TypeKind Type::GetKind() const {
		return m_Kind;
	}

	AccessSpecifier Type::GetAccess() const {
		return m_Access;
	}

	size_t Type::GetSize() const {
		return m_SizeInBytes;
	}

	bool Type::IsPrimitive() const {
		return m_Kind == TypeKind::PRIMITIVE;
	}

	bool Type::IsStruct() const {
		return m_Kind == TypeKind::STRUCT;
	}

	bool Type::IsClass() const {
		return m_Kind == TypeKind::CLASS;
	}

	bool Type::IsPolymorphic() const {
		return m_Polymorphic;
	}

	bool Type::IsValid() const {
		std::string qName = m_QualifiedName.FullQualified();
		return qName.find("INVALID") == std::string::npos
		       && qName.find("UNKNOWN") == std::string::npos;
	}

	bool Type::IsSTL() const {
		const Namespace& ns = m_QualifiedName.GetNamespace();
		return ns.size() > 0 && ns[0] == "std";
	}

	bool Type::IsSequentialContainer() const {
		const std::string& name = m_QualifiedName.GetName();
		return name == "vector" || name == "deque" || name == "array" || name == "list" || name == "forward_list";
	}

	bool Type::IsStaticArray() const {
		const std::string& name = m_QualifiedName.GetName();
		return name == "array";
	}

	bool Type::IsAssociativeContainer() const {
		return false;
	}

	bool Type::IsContainer() const {
		return IsSequentialContainer() || IsAssociativeContainer();
	}

	const std::vector<Field>& Type::GetFields() const {
		return m_Fields;
	}

	const std::vector<TemplateArgument> Type::GetTemplateArguments() const {
		return m_TemplateArguments;
	}

	const std::vector<BaseType>& Type::GetBaseTypes() const {
		return m_BaseTypes;
	}

	const std::vector<TypeID>& Type::GetDerivedTypes() const {
		return m_DerivedTypes;
	}

	Container* Type::GetContainer() const {
		return m_Container;
	}

	void* Type::Allocate(void* ptr) const {
		if (ptr == 0)
			ptr = malloc(m_SizeInBytes);
		memset(ptr, 0, m_SizeInBytes);

		if (m_Constructor)
			//new (ptr) T;
			ptr = m_Constructor(ptr);

		return ptr;
	}

	void Type::Delete(void* ptr) const {
		if (m_Destructor && ptr)
			//ptr->~T();
			m_Destructor(ptr);

		free(ptr);
	}

	void Type::SetKind(const TypeKind kind) {
		m_Kind = kind;
	}

	void Type::SetAccess(const AccessSpecifier access) {
		m_Access = access;
	}

	void Type::SetSize(const size_t bytes) {
		m_SizeInBytes = bytes;
	}

	void Type::SetPolymorphic(const bool polymorphic) {
		m_Polymorphic = polymorphic;
	}

	void Type::SetHasDefaultConstructor(const bool hasDefaultConstructor) {
		m_HasDefaultConstructor = hasDefaultConstructor;
	}

	void Type::SetHasDefaultDestructor(const bool hasDefaultDestructor) {
		m_HasDefaultDestructor = hasDefaultDestructor;
	}

	void Type::SetConstructor(const Constructor constructor) {
		m_Constructor = constructor;
	}

	void Type::SetDestructor(const Destructor destructor) {
		m_Destructor = destructor;
	}

	void Type::SetContainer(Container* container) {
		m_Container = container;
	}

	void Type::AddBaseType(const QualifiedType& baseType, const AccessSpecifier access) {
		m_BaseTypes.push_back(BaseType{ baseType, access });
	}

	void Type::AddDerivedType(TypeID typeID) {
		m_DerivedTypes.push_back(typeID);
	}

	void Type::AddTemplateArgument(const TemplateArgument& arg, const int pos) {
		if (pos == -1)
			m_TemplateArguments.push_back(arg);
		else
			m_TemplateArguments.insert(m_TemplateArguments.begin() + pos, arg);
	}

	void Type::AddField(Field& field) {
		field.SetOwner(m_ID);
		m_Fields.emplace_back(field);
	}

	void Type::AddMethod(Method& method) {
		method.SetOwner(m_ID);
		m_Methods.emplace_back(method);
	}

	bool Type::HasSize() const {
		const std::string& name = m_QualifiedName.GetName();
		return name != "void" && name != "_Alloc_hider";
	}

	const std::vector<Method>& Type::GetMethods() const {
		return m_Methods;
	}

	void Type::SetArraySize(const size_t arraySize) {
		m_ArraySize = arraySize;
	}

	size_t Type::GetArraySize() const {
		return m_ArraySize;
	}
}
