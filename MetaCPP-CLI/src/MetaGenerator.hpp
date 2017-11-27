#include <vector>
#include <string>

#include <clang/Tooling/Tooling.h>

#include "MetaCPP/Storage.hpp"

namespace metacpp {
	class MetaGenerator {
	public:
		MetaGenerator(std::vector<std::string> sources, std::vector<std::string> flags);
		~MetaGenerator();

		void Generate(Storage* storage);

	private:
		clang::tooling::FixedCompilationDatabase* m_CompilationDatabase;
		clang::tooling::ClangTool* m_ClangTool;
	};
}
