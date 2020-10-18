#ifndef METACPP_TEMPLATES_HPP
#define METACPP_TEMPLATES_HPP

#include <string>

namespace metacpp { namespace templates {

#define METACPP_TEMPLATE(name) const std::string name = std::string("") +

	/* Code templates */
	METACPP_TEMPLATE(header)
	#include "templates/header.template"
	;

	METACPP_TEMPLATE(source)
	#include "templates/source.template"
	;
} }

#endif