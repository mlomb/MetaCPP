#ifndef METACPP_TYPEHASH_HPP
#define METACPP_TYPEHASH_HPP

#include <string>

namespace metacpp {
	typedef unsigned TypeHash;

	TypeHash HashFromString(const std::string& input);
}

#endif