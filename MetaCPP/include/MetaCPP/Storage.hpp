#ifndef METACPP_STORAGE_HPP
#define METACPP_STORAGE_HPP

#include <unordered_map>

#include "Type.hpp"
#include "TypeInfo.hpp"

namespace metacpp {
	class Storage {
	public:
		typedef std::function<void*(void*)> DynamicCast;

		Storage();
		~Storage();

		TypeID AssignTypeID(const std::string& name, const TypeID typeId = 0);
		void AddType(Type* type);
		void AddDynamicCast(TypeID base, TypeID derived, const DynamicCast& dc);

		bool HasType(const TypeID typeId) const;
		bool IsDerived(const TypeID derived, const TypeID base) const;

		TypeID GetTypeID(const std::string& name) const;
		Type* GetType(const TypeID typeId) const;
		Type* GetType(const std::string& name) const;
		template<typename T>
		Type* GetType() const;

		std::vector<Field> GetAllFields(const Type* type);
		std::pair<const Type*, void*> ResolveDerivedType(const Type* baseClass, void* base_ptr);
		Type* FindDerivedTypeWithName(const Type* base, const std::string& derived_name);

	private:
		std::unordered_map<std::string, TypeID> m_IDs;
		std::unordered_map<TypeID, Type*> m_Types;
		std::unordered_map<int, DynamicCast> m_DynamicCasts;

		TypeID m_NextID = 1;

		friend class MetaExporter;
	};

	template<typename T>
	inline Type* Storage::GetType() const
	{
		return TypeInfo<T>::TYPE;
	}
}

#endif