// File: src/modules/OpenAIClient.cpp

#include "../headers/NetworkUtils.h"
#include "../headers/OpenAIClient.h"

namespace OpenAIClient
{
	Client::Client(const std::string &apiKey, const std::string &apiUrl, const std::string &model, int maxTokens, double temperature)
		: apiKey(apiKey), apiUrl(apiUrl), model(model), maxTokens(maxTokens), temperature(temperature) {}

	std::string Client::sendPrompt(const std::string &prompt)
	{
		return NetworkUtils::callOpenAI(prompt, apiUrl, apiKey, maxTokens, model, temperature);
	}
}
