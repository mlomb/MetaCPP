#include "MetaCPP/TypeHash.hpp"

#include <CRC.h>

namespace metacpp {
	TypeHash HashFromString(const std::string& input)
	{
		return CRC::Calculate(input.data(), input.size(), CRC::CRC_32());
	}
}
