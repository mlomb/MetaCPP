#ifndef METACPP_IDS_HPP
#define METACPP_IDS_HPP

#include <string>

namespace metacpp {
	typedef unsigned ID;

	typedef ID TypeID;
	typedef ID FieldID;
	typedef ID MethodID;

	enum IDTypes { TYPE = 0, FIELD, METHOD };
	const std::string IDTypes_Names[]{ "Types", "Fields", "Methods" };

	template<typename T>
	struct TypeIDs {
		static TypeID ID;
	};

	template<typename T>
	TypeID TypeIDs<T>::ID = 0;
}

#endif