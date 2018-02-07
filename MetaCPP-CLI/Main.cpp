#include <iostream>
#include <string>

#include <llvm/Support/CommandLine.h>

#include "ASTScraper.hpp"
#include "MetaExporter.hpp"
#include "ScraperTool.hpp"
#include "MetaCPP/Storage.hpp"

using namespace llvm;

static cl::opt<std::string> InputSource(cl::Positional, cl::Required, "in-source", cl::desc("Specify input filename"), cl::value_desc("filename"));
static cl::opt<std::string> OutputHeader("out-header", cl::desc("Specify header output filename"), cl::value_desc("filename"), cl::init("output.hpp"));
static cl::opt<std::string> OutputSource("out-source", cl::desc("Specify source output filename"), cl::value_desc("filename"), cl::init("output.cpp"));

static cl::list<std::string> CompilerFlags("flag", cl::desc("Compiler flags"), cl::value_desc("flags"), cl::ZeroOrMore);

static cl::opt<std::string> ReflectionAnnotation("reflection-annotation", cl::desc("Only reflect types that contain this annotation"), cl::value_desc("name"));

int main(int argc, const char** argv) {
	cl::ParseCommandLineOptions(argc, argv, "MetaCPP");

	metacpp::Storage* storage = new metacpp::Storage();

	// Generate Metadata
	{
		metacpp::ASTScraper::Configuration configuration;
		configuration.AnnotationRequired = ReflectionAnnotation;

		metacpp::ASTScraper* scraper = new metacpp::ASTScraper(storage, configuration);
		metacpp::ScraperTool tool(InputSource, CompilerFlags);
		tool.Run(scraper);
		delete scraper;
	}

	// Export Metadata
	{
		metacpp::MetaExporter exporter(storage);
		exporter.Export(InputSource, OutputHeader, OutputSource);
	}

	return 0;
}
