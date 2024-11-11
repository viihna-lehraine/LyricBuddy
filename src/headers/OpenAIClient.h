// File: src/headers/OpenAIClient.h

#ifndef OPENAI_CLIENT_H
#define OPENAI_CLIENT_H

#include <string>

namespace OpenAIClient
{
	class Client
	{
	private:
		std::string apiKey;
		std::string apiUrl;
		std::string model;
		int maxTokens;
		double temperature;

	public:
		Client(const std::string &apiKey, const std::string &apiUrl, const std::string &model, int maxTokens, double temperature);

		std::string sendPrompt(const std::string &prompt);
	};
}

#endif // OPENAI_CLIENT_H
