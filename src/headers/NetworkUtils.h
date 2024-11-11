// File: src/headers/NetworkUtils.h

#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <json/json.h>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <sstream>
#include <stdexcept>

namespace NetworkUtils
{
	constexpr char API_URL[] = "https://api.openai.com/v1/chat/completions";

	std::string callOpenAI(const std::string &prompt, const std::string &apiUrl, const std::string &apiKey, int maxTokens, const std::string &model, double temperature);
	std::string parseAIResponse(const std::string &response);
}

#endif // NETWORK_UTILS_H
