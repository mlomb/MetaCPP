#include <iostream>
#include <string>

#include <llvm/Support/CommandLine.h>

#include "MetaGenerator.hpp"
#include "MetaCPP/Storage.hpp"

using namespace llvm;

static cl::list<std::string> InputSource(cl::Positional, cl::Required, "in-source", cl::desc("Specify input filename"), cl::value_desc("filename"));
static cl::opt<std::string> OutputHeader("out-header", cl::desc("Specify header output filename"), cl::value_desc("filename"), cl::init("output.hpp"));
static cl::opt<std::string> OutputSource("out-source", cl::desc("Specify source output filename"), cl::value_desc("filename"), cl::init("output.cpp"));

static cl::list<std::string> AdditionalFlags("flag", cl::desc("Additional compiler flag"), cl::value_desc("flags"), cl::ZeroOrMore);

int main(int argc, const char** argv) {
	cl::ParseCommandLineOptions(argc, argv, "MetaCPP");

	//AdditionalFlags.push_back("-DTEST");

	metacpp::Storage* storage = new metacpp::Storage();

	metacpp::MetaGenerator* generator = new metacpp::MetaGenerator(InputSource, AdditionalFlags);
	generator->Generate(storage);

	storage->dump();

	std::cout << "OK" << std::endl;

	return 1;
}
