#include <string>

#include <llvm/Support/CommandLine.h>

#include "MetaGenerator.hpp"

using namespace llvm;

static cl::list<std::string> InputSource(cl::Positional, cl::Required, "in-source", cl::desc("Specify input filename"), cl::value_desc("filename"));
static cl::opt<std::string> OutputHeader("out-header", cl::desc("Specify header output filename"), cl::value_desc("filename"), cl::init("output.hpp"));
static cl::opt<std::string> OutputSource("out-source", cl::desc("Specify source output filename"), cl::value_desc("filename"), cl::init("output.cpp"));

static cl::list<std::string> AdditionalFlags("flags", cl::desc("Additional compiler flags"), cl::value_desc("flags"));

int main(int argc, const char** argv) {
	cl::ParseCommandLineOptions(argc, argv, "MetaCPP");

	AdditionalFlags.push_back("-D__GENERATE_REFLECTION__");

	metacpp::MetaGenerator* generator = new metacpp::MetaGenerator(InputSource, AdditionalFlags);
	generator->Generate();

	return 0;
}
