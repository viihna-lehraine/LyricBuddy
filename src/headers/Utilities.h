// File: src/headers/Utilities.h

#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>

namespace Utilities
{
	bool validateAPIKey(const std::string &apiKey);

	bool validateResponse(const std::string &response);
}

#endif // UTILITIES_H
