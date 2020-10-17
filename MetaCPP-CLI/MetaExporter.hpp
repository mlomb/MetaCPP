#ifndef METACPP_METAEXPORTER_HPP
#define METACPP_METAEXPORTER_HPP

#include <vector>
#include <string>

#include <mustache.hpp>
using namespace kainjow;

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	class Storage;
	class Type;
    class QualifiedType;
	class Field;

	class MetaExporter {
	public:
		MetaExporter(const Storage* storage);
		
		void Export(const std::string& inputSource, const std::string& outputHeader, const std::string& outputSource);
		
	private:
		std::string GenerateSourceFile(const std::vector<std::string>& includes, mustache::data& storageData);
		std::string GenerateHeaderFile(const std::vector<std::string>& includes, mustache::data& storageData);

		mustache::data ExportStorage();
		mustache::data ExportType(const Type* type);
        mustache::data ExportQualifiedType(const QualifiedType& qtype);
        mustache::data ExportTemplateArgument(const TemplateArgument& argument);
		mustache::data ExportField(const Field& field);

		void SetIncludes(const std::vector<std::string>& includes, mustache::data& data);

		const Storage* m_Storage;
	};
}

#endif