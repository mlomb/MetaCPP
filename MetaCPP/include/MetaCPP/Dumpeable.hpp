#ifndef METACPP_DUMPEABLE_HPP
#define METACPP_DUMPEABLE_HPP

#include <ostream>

namespace metacpp {
	class Storage;

	class Dumpeable {
	public:
		void dump(std::ostream& o);
		void dump();

	protected:
		void indent(unsigned int spaces = 2);
		void deindent(unsigned int spaces = 2);

		static std::string indentation;
		static Storage* dump_storage;

	private:
		virtual void dump_obj(std::ostream& o) = 0;
	};
}

#endif