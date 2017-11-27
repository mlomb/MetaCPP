#ifndef METACPP_TYPEHASH_HPP
#define METACPP_TYPEHASH_HPP

#include <string>

namespace metacpp {
	typedef unsigned TypeHash;

	template<typename T>
	struct TypeHashes {
		static TypeHash Hash;
	};

	template<typename T>
	TypeHash TypeHashes<T>::Hash = 0;

	TypeHash HashFromString(const std::string& input);
}

#endif