#include "MetaCPP/Exportable.hpp"

#include <iostream>

namespace metacpp {
	Storage* Exportable::dumping_storage = 0;

	void Exportable::dump(std::ostream& o)
	{
		mustache::mustache mstch(dumpTemplate());
		mstch.set_custom_escape([](const std::string& str) { return str; });

		mustache::data data = asMustache();
		std::string rendered = mstch.render(data);
		o << rendered;
	}

	void Exportable::dump()
	{
		dump(std::cout);
	}

	mustache::data Exportable::asMustache() const
	{
		return mustache::data();
	}

	std::string Exportable::dumpTemplate()
	{
		return "";
	}
}
