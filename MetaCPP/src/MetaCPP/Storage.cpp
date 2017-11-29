#include "MetaCPP/Storage.hpp"

#include <iostream>
#include <algorithm>

namespace metacpp {

	Storage::Storage() {
		for (int i = IDTypes::TYPE; i < IDTypes::METHOD + 1; i++) {
			IDTypes idType = static_cast<IDTypes>(i);
			m_IDs[idType];
			m_Objects[idType];
		}

		dump_storage = this;
	}

	ID Storage::getID(const IDTypes idType, const std::string& name)
	{
		auto& ids_map = m_IDs[idType];
		auto it = ids_map.find(name);
		return it != ids_map.end() ? (*it).second : 0;
	}

	TypeID Storage::getTypeID(const std::string& name)
	{
		return getID(IDTypes::TYPE, name);
	}

	FieldID Storage::getFieldID(const std::string& name)
	{
		return getID(IDTypes::FIELD, name);
	}

	void* Storage::get(const IDTypes idType, const ID id) const
	{
		auto& objs_map = (*m_Objects.find(idType)).second;
		auto it = objs_map.find(id);
		return it != objs_map.end() ? (*it).second : nullptr;
	}

	Type* Storage::getType(const TypeID typeId) const
	{
		return static_cast<Type*>(get(IDTypes::TYPE, typeId));
	}

	Field* Storage::getField(const FieldID fieldId) const
	{
		return static_cast<Field*>(get(IDTypes::FIELD, fieldId));
	}

	ID Storage::assignID(const IDTypes idType, const std::string& name, const ID id)
	{
		ID _id = getID(idType, name);
		if (_id != 0 && (id == 0 || _id == id))
			return _id;

		_id = id;
		if(id == 0)
			_id = m_NextID++;
		m_IDs[idType].insert(std::make_pair(name, _id));
		return _id;
	}

	TypeID Storage::assignTypeID(const std::string& name, const TypeID typeId)
	{
		return assignID(IDTypes::TYPE, name, typeId);
	}

	FieldID Storage::assignFieldID(const std::string& name, const FieldID fieldId)
	{
		return assignID(IDTypes::FIELD, name, fieldId);
	}

	void Storage::add(const IDTypes idType, const ID id, void* ptr)
	{
		m_Objects[idType].insert(std::pair<ID, void*>(id, ptr));
	}

	void Storage::addType(Type* type)
	{
		add(IDTypes::TYPE, type->getID(), type);
	}

	void Storage::addField(Field* field)
	{
		add(IDTypes::FIELD, field->getID(), field);
	}

	bool Storage::has(const IDTypes idType, const ID id) const
	{
		auto& objs_map = (*m_Objects.find(idType)).second;
		return objs_map.count(id);
	}

	bool Storage::hasType(const TypeID typeId) const
	{
		return has(IDTypes::TYPE, typeId);
	}

	bool Storage::hasField(const FieldID fieldId) const
	{
		return has(IDTypes::FIELD, fieldId);
	}

	void Storage::dump_obj(std::ostream& o)
	{
		o << indentation << "Storage:" << std::endl;
		indent();
		{
			// IDs
			o << indentation << "IDs:" << std::endl;
			indent();
			{
				for (int i = IDTypes::TYPE; i < IDTypes::METHOD + 1; i++) {
					IDTypes idType = static_cast<IDTypes>(i);
					o << indentation << IDTypes_Names[idType] << ":" << std::endl;

					indent();
					{
						auto& ids = m_IDs[idType];
						for (auto& kv : ids)
							o << indentation << kv.second << "=" << kv.first << std::endl;
					}
					deindent();
				}
			}
			deindent();

			// Types
			auto& types = m_Objects[IDTypes::TYPE];
			o << indentation << IDTypes_Names[IDTypes::TYPE] << " (" << types.size() << "):" << std::endl;
			indent();
			{
				for (auto& kv : types)
					static_cast<Type*>(kv.second)->dump(o);
			}
			deindent();
		}
		deindent();
	}

	/*
	template<typename T>
	Type* Storage::getType()
	{
	return getType(TypeIDs<T>::ID);
	}
	*/
}