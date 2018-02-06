#include "MetaCPP/Storage.hpp"

#include <iostream>
#include <algorithm>

#include "MetaCPP/Templates.hpp"

namespace metacpp {

	Storage::Storage() {
		dumping_storage = this;
	}

	TypeID Storage::getTypeID(const std::string& name) const
	{
		auto it = m_IDs.find(name);
		return it != m_IDs.end() ? (*it).second : 0;
	}

	Type* Storage::getType(const TypeID typeId) const
	{
		auto it = m_Types.find(typeId);
		return it != m_Types.end() ? (*it).second : nullptr;
	}

	Type* Storage::getType(const std::string& name) const
	{
		return getType(getTypeID(name));
	}

	TypeID Storage::assignTypeID(const std::string& name, const TypeID typeId)
	{
		TypeID _id = getTypeID(name);
		if (_id != 0 && (typeId == 0 || _id == typeId))
			return _id;

		_id = typeId;
		if (typeId == 0)
			_id = m_NextID++;
		else
			m_NextID = std::max(m_NextID, _id) + 1;
		m_IDs.insert(std::make_pair(name, _id));
		return _id;
	}

	void Storage::addType(Type* type)
	{
		m_Types.insert(std::make_pair(type->getID(), type));
	}

	void Storage::addDynamicCast(TypeID base, TypeID derived, const DynamicCast& dc)
	{
		int index = (size_t)base << 32 | (unsigned int)derived;
		m_DynamicCasts.insert(std::make_pair(index , dc));
	}

	bool Storage::hasType(const TypeID typeId) const
	{
		return m_Types.count(typeId);
	}
	
	bool Storage::isDerived(const TypeID derived, const TypeID base) const
	{
		Type* derivedType = getType(derived);
		if (derivedType == 0)
			return false;

		auto& baseTypes = derivedType->getBaseTypes();
		for (auto& baseType : baseTypes) {
			if (baseType.type->getTypeID() == base)
				return true;
			if (isDerived(baseType.type->getTypeID(), base))
				return true;
		}
		return false;
	}

	std::vector<Field*> Storage::getAllFields(const Type* type)
	{
		std::vector<Field*> fields;

		for (auto& baseType : type->getBaseTypes()) {
			const std::vector<Field*>& parentFields = getAllFields(getType(baseType.type->getTypeID()));
			fields.reserve(fields.size() + parentFields.size());
			fields.insert(fields.end(), parentFields.begin(), parentFields.end());
		}

		const std::vector<Field*>& typeFields = type->getFields();

		fields.reserve(fields.size() + typeFields.size());
		fields.insert(fields.end(), typeFields.begin(), typeFields.end());

		return fields;
	}

	std::pair<const Type*, void*> Storage::ResolveDerivedType(const Type* baseClass, void* base_ptr) {
		for (const TypeID derived_id : baseClass->getDerivedTypes()) {
			int index = (size_t)baseClass->getID() << 32 | (unsigned int)derived_id;
			auto it = m_DynamicCasts.find(index);
			if (it != m_DynamicCasts.end()) {
				const DynamicCast& dc = (*it).second;
				if (void* derived_ptr = dc(base_ptr)) {
					return ResolveDerivedType(getType(derived_id), derived_ptr);
				}
			}
		}
		return std::make_pair(baseClass, base_ptr);
	}

	Type* Storage::FindDerivedTypeWithName(const Type* base, const std::string& derived_name)
	{
		for (const TypeID derived_id : base->getDerivedTypes()) {
			Type* derived_type = getType(derived_id);
			if (derived_type->getQualifiedName().getTemplatedName() == derived_name)
				return derived_type;
			FindDerivedTypeWithName(derived_type, derived_name);
		}
		return nullptr;
	}

	mustache::data Storage::asMustache() const
	{
		mustache::data data;

		// IDs
		mustache::data ids{ mustache::data::type::list };
		for (const auto& kv : m_IDs) {
			mustache::data entry;
			entry["id"] = std::to_string(kv.second);
			entry["qualifiedName"] = kv.first;
			ids << entry;
		}
		data["ids"] = mustache::data{ ids };

		// Types
		mustache::data types{ mustache::data::type::list };
		for (const auto& kv : m_Types)
			types << kv.second->asMustache();
		data["types"] = mustache::data{ types };

		return data;
	}

	std::string Storage::dumpTemplate()
	{
		dumping_storage = this;
		return templates::dump_storage;
	}
}