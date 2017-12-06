#include "MetaCPP/Runtime.hpp"

namespace metacpp {
	Storage* Runtime::s_Storage = 0;

	Storage* Runtime::Storage()
	{
		if (s_Storage == 0)
			s_Storage = new metacpp::Storage();
		return s_Storage;
	}
}
