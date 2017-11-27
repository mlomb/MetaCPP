#ifndef METACPP_TYPE_HPP
#define METACPP_TYPE_HPP

#include <vector>

#include "TypeHash.hpp"
#include "Field.hpp"
//#include "Method.hpp"

namespace metacpp {
	class Type {
	public:
		Type(TypeHash hash) : id(hash) {  }

		const TypeHash id;
		std::string name;
		unsigned int size_bytes;
		bool is_struct;
		bool is_primitive;

		std::vector<Field*> fields;
		//std::vector<Method*> methods;
	};
}

#endif