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

	bool Storage::hasType(const TypeID typeId) const
	{
		return m_Types.count(typeId);
	}
	
	bool Storage::isDerived(const TypeID derived, const TypeID base) const
	{
		Type* derivedType = getType(derived);
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