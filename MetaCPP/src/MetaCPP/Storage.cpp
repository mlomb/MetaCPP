#include "MetaCPP/Storage.hpp"

namespace metacpp {
	void Storage::AddType(Type* type)
	{
		m_IDs.emplace(std::make_pair(type->name, type->id));
		m_Types.emplace(std::make_pair(type->id, type));
	}

	TypeHash Storage::GetTypeID(std::string name)
	{
		auto it = m_IDs.find(name);
		if (it != m_IDs.end())
			return (*it).second;
		return 0;
	}
	
	bool Storage::HasType(TypeHash id)
	{
		return m_Types.find(id) != m_Types.end();
	}

	Type* Storage::GetType(TypeHash id)
	{
		auto it = m_Types.find(id);
		if (it != m_Types.end())
			return (*it).second;
		return 0;
	}

}
