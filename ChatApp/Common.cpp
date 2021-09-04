#include "Common.h"

std::string Common::IP2String(const IPDATA& ip)
{
	return std::string()
		.append(std::to_string(ip.d1))
		.append(".")
		.append(std::to_string(ip.d2))
		.append(".")
		.append(std::to_string(ip.d3))
		.append(".")
		.append(std::to_string(ip.d4));
}

bool Common::ApplicationExecution = true;
