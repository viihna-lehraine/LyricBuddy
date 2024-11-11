// File: src/headers/EnvParser.h

#ifndef ENV_PARSER_H
#define ENV_PARSER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <json/json.h>
#include <sstream>
#include <stdexcept>

namespace EnvParser
{
	std::string getAPIKeyFromSOPS(const std::string &filename);

	std::string getOrPromptForAPIKey();

	std::unordered_map<std::string, std::string> loadEnvironment();

	std::unordered_map<std::string, std::string> parseEnvFile(const std::string &envFilePath);
}

#endif // ENV_PARSER_H
