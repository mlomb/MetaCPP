#ifndef METACPP_STORAGE_HPP
#define METACPP_STORAGE_HPP

#include <vector>
#include <map>

#include "Type.hpp"

namespace metacpp {
	class Storage {
	public:
		TypeHash GetTypeID(std::string name);
		Type* GetType(TypeHash id);
		bool HasType(TypeHash id);

		void AddType(Type* type);

	private:
		std::map<std::string, TypeHash> m_IDs;
		std::map<TypeHash, Type*> m_Types;
	};
}

#endif