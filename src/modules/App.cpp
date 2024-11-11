// File: src/modules/App.cpp

#include "../headers/App.h"
#include "../headers/EnvParser.h"
#include "../headers/Interaction.h"
#include "../headers/OpenAIClient.h"

namespace App
{
	OpenAIClient::Client initializeClient(const std::unordered_map<std::string, std::string> &envVars)
	{
		std::string apiKey = EnvParser::getOrPromptForAPIKey();

		return OpenAIClient::Client(
			apiKey,
			envVars.at("API_URL"),
			envVars.at("MODEL"),
			std::stoi(envVars.at("MAX_TOKENS")),
			std::stod(envVars.at("TEMPERATURE")));
	}
}
