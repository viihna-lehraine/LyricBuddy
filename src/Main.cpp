// File: src/Main.cpp

#include "headers/App.h"
#include "headers/EnvParser.h"
#include "headers/Interaction.h"
#include "headers/Main.h"
#include "headers/NetworkUtils.h"
#include "headers/OpenAIClient.h"
#include "headers/Utilities.h"

int main()
{
	try
	{
		auto envVars = EnvParser::loadEnvironment();

		OpenAIClient::Client client = App::initializeClient(envVars);

		Interaction::runMainLoop(client);
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;

		return 1;
	}

	return 0;
}