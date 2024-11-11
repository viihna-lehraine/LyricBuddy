// File: src/modules/Utilities.cpp

#include "../headers/NetworkUtils.h"
#include "../headers/Utilities.h"

namespace Utilities
{
	bool validateAPIKey(const std::string &apiKey, std::string &errorDetails)
	{
		// validate key format
		const std::regex keyPattern("^sk-[a-zA-Z0-9]{32,}$");

		if (!std::regex_match(apiKey, keyPattern))
		{
			errorDetails = "Invalid API Key format. Ensure it starts with 'sk-' and has the correct length.";
			return false;
		}

		// test key with minimal API call
		try
		{
			std::string testPrompt = "Test";

			NetworkUtils::callOpenAI(testPrompt, NetworkUtils::API_URL, apiKey, 1, "gpt-3.5-turbo", 0.0);

			return true; // valid key
		}
		catch (const std::exception &ex)
		{
			errorDetails = ex.what();

			return false; // invalid key
		}
	}

	bool validateResponse(const std::string &response)
	{
		return response == "yes" || response == "y" || response == "no" || response == "n";
	}
}
