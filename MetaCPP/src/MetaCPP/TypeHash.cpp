#include "MetaCPP/TypeHash.hpp"

#define CRCPP_USE_CPP11
#include <CRC.h>

namespace metacpp {
	TypeHash HashFromString(const std::string& input)
	{
		return CRC::Calculate(input.data(), input.size(), CRC::CRC_32());
	}
}
