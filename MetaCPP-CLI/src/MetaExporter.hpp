#ifndef METACPP_METAEXPORTER_HPP
#define METACPP_METAEXPORTER_HPP

#include <vector>
#include <string>

namespace metacpp {
	class Storage;

	class MetaExporter {
	public:
		MetaExporter(const Storage* storage);
		
		void Export(const std::string& inputSource, const std::string& outputHeader, const std::string& outputSource);
		
	private:
		std::string GenerateSourceFile(const std::vector<std::string>& includes);
		const Storage* m_Storage;
	};
}

#endif