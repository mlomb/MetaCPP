#include "MetaExporter.hpp"

#include <iostream>
#include <fstream>

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

		mustache::data storageData = m_Storage->asMustache();

		out_source << GenerateSourceFile({ outputHeader }, storageData);
		out_header << GenerateHeaderFile({ inputSource }, storageData);
	}

	std::string MetaExporter::GenerateSourceFile(const std::vector<std::string>& includes, mustache::data& storageData)
	{
		mustache::mustache source_template(templates::source);
		source_template.set_custom_escape([](const std::string& str) { return str; });

		SetIncludes(includes, storageData);

		return source_template.render(storageData);
	}

	std::string MetaExporter::GenerateHeaderFile(const std::vector<std::string>& includes, mustache::data& storageData)
	{
		mustache::mustache header_template(templates::header);
		header_template.set_custom_escape([](const std::string& str) { return str; });

		SetIncludes(includes, storageData);

		return header_template.render(storageData);
	}

	void MetaExporter::SetIncludes(const std::vector<std::string>& includes, mustache::data& data)
	{
		mustache::data inclds{ mustache::data::type::list };
		for (const std::string& file : includes)
			inclds << mustache::data{ "file", file };
		data["includes"] = mustache::data{ inclds };
	}
}
