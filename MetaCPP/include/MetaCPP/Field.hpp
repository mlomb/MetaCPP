#ifndef METACPP_FIELD_HPP
#define METACPP_FIELD_HPP

#include <vector>
#include <string>

#include "TypeHash.hpp"

namespace metacpp {
	class Field {
	public:
		TypeHash type;
		std::string name;
		TypeHash owner_type;
	};
}

#endif