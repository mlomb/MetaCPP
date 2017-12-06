#ifndef METACPP_EXPORTABLE_HPP
#define METACPP_EXPORTABLE_HPP

#include <ostream>

#include <mustache.hpp>
using namespace kainjow; // mustache

namespace metacpp {
	class Storage;

	class Exportable {
	public:
		void dump(std::ostream& o);
		void dump();

		virtual mustache::data asMustache() const;
		// TODO asJSON etc

	private:
		virtual std::string dumpTemplate();

	protected:
		static Storage* dumping_storage;
	};
}

#endif