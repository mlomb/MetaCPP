#ifndef METACPP_STORAGE_HPP
#define METACPP_STORAGE_HPP

#include <map>

#include "Type.hpp"
#include "TypeInfo.hpp"
#include "Exportable.hpp"

namespace metacpp {
	class Storage : public Exportable {
	public:
		typedef std::function<void*(void*)> DynamicCast;

		Storage();

		TypeID assignTypeID(const std::string& name, const TypeID typeId = 0);
		void addType(Type* type);
		void addDynamicCast(TypeID base, TypeID derived, const DynamicCast& dc);

		TypeID getTypeID(const std::string& name) const;
		Type* getType(const TypeID typeId) const;
		Type* getType(const std::string& name) const;
		template<typename T>
		Type* getType() const;

		bool hasType(const TypeID typeId) const;
		bool isDerived(const TypeID derived, const TypeID base) const;

		std::vector<Field*> getAllFields(const Type* type);
		std::pair<const Type*, void*> ResolveDerivedType(const Type* baseClass, void* base_ptr);
		Type* FindDerivedTypeWithName(const Type* base, const std::string& derived_name);

		mustache::data asMustache() const override;
	private:
		std::string dumpTemplate() override;

		std::unordered_map<std::string, TypeID> m_IDs;
		std::unordered_map<TypeID, Type*> m_Types;
		std::unordered_map<int, DynamicCast> m_DynamicCasts;

		TypeID m_NextID = 1;
	};

	template<typename T>
	inline Type* Storage::getType() const
	{
		return TypeInfo<T>::TYPE;
	}
}

#endif