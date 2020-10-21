#ifndef METACPP_TYPEINFO_HPP
#define METACPP_TYPEINFO_HPP

#include "TypeID.hpp"
#include "Type.hpp"

namespace metacpp {
	template<typename T>
	struct TypeInfo {
		static TypeID ID;
		static Type* TYPE;
	};

	template<typename T> TypeID TypeInfo<T>::ID = 0;
	template<typename T> Type* TypeInfo<T>::TYPE = nullptr;
}

#endif