#include "MetaExporter.hpp"

#include <iostream>
#include <fstream>

#include <mustache.hpp>
using namespace kainjow;

#include "MetaCPP/Templates.hpp"
#include "MetaCPP/Storage.hpp"

namespace metacpp {
	MetaExporter::MetaExporter(const Storage* storage)
		: m_Storage(storage)
	{
	}

	void MetaExporter::Export(const std::string& inputSource, const std::string& outputHeader, const std::string& outputSource)
	{
		std::ofstream out_header(outputHeader);
		std::ofstream out_source(outputSource);

		std::vector<std::string> sourceIncludes{
			inputSource,
			outputHeader
		};

		out_source << GenerateSourceFile(sourceIncludes);
	}

	std::string MetaExporter::GenerateSourceFile(const std::vector<std::string>& includes)
	{
		mustache::mustache source_template(templates::source);
		source_template.set_custom_escape([](const std::string& str) { return str; });

		mustache::data source_data = m_Storage->asMustache();

		mustache::data inclds{ mustache::data::type::list };
		for (const std::string& file : includes)
			inclds << mustache::data{ "file", file };
		source_data["includes"] = mustache::data{ inclds };

		return source_template.render(source_data);
	}
}
