#include "MetaCPP/Dumpeable.hpp"

#include <iostream>

namespace metacpp {
	std::string Dumpeable::indentation = "";
	Storage* Dumpeable::dump_storage = nullptr;

	void Dumpeable::dump(std::ostream& o)
	{
		dump_obj(o);
	}

	void Dumpeable::dump()
	{
		dump(std::cout);
	}

	void Dumpeable::indent(unsigned int spaces)
	{
		indentation.append(std::string(spaces, ' '));
	}

	void Dumpeable::deindent(unsigned int spaces)
	{
		indentation.erase(0, spaces);
	}
}