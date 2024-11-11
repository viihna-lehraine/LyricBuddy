// File: src/modules/Interaction.cpp

#include "../headers/Interaction.h"
#include "../headers/NetworkUtils.h"
#include "../headers/OpenAIClient.h"
#include "../headers/Utilities.h"

namespace Interaction
{
	void runMainLoop(OpenAIClient::Client &client)
	{
		bool continuePrompting = true;

		while (continuePrompting)
		{
			std::string mainPrompt, songDetails;

			std::cout << "Enter a lyric idea prompt: ";
			std::getline(std::cin, mainPrompt);
			std::cout << "Describe the song's style, tempo, mood, etc.: ";
			std::getline(std::cin, songDetails);

			std::string fullPrompt = "Main idea: " + mainPrompt + "\nStyle details: " + songDetails;
			std::string response = client.sendPrompt(fullPrompt);

			std::cout << NetworkUtils::parseAIResponse(response) << std::endl;

			std::string userChoice;

			do
			{
				std::cout << "Would you like to enter another prompt? (yes/y or no/n): ";
				std::getline(std::cin, userChoice);
				std::transform(userChoice.begin(), userChoice.end(), userChoice.begin(), ::tolower);
			} while (!Utilities::validateResponse(userChoice));

			continuePrompting = (userChoice == "yes" || userChoice == "y");
		}

		std::cout << "Goodbye!" << std::endl;
	}
}
