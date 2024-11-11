// File: src/headers/App.h

#ifndef APP_H
#define APP_H

#include "OpenAIClient.h"
#include <unordered_map>
#include <string>

namespace App
{
	OpenAIClient::Client initializeClient(const std::unordered_map<std::string, std::string> &envVars);
}

#endif // APP_H
