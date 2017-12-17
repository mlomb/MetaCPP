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

	/* Dump templates */
	METACPP_TEMPLATE(dump_storage)
	#include "templates/dump_storage.template"
	;

	METACPP_TEMPLATE(dump_type)
	#include "templates/dump_type.template"
	;
} }

#endif