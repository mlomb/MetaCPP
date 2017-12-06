#ifndef METACPP_RUNTIME_HPP
#define METACPP_RUNTIME_HPP

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	namespace generated { extern void Load(Storage* storage); }

	class Runtime {
	public:
		static Storage* Storage();

	private:
		static metacpp::Storage* s_Storage;
	};
}

#endif