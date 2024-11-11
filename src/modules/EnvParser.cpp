// File: src/modules/EnvParser.cpp

#include "../headers/EnvParser.h"
#include "../headers/Utilities.h"

namespace EnvParser
{
	std::string getAPIKeyFromSOPS(const std::string &filename)
	{
		std::string command = "sops -d " + filename;

		FILE *pipe = popen(command.c_str(), "r");

		if (!pipe)
		{
			throw std::runtime_error("Failed to run SOPS decryption command.");
		}

		char buffer[128];

		std::string decryptedContent;

		while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		{
			decryptedContent += buffer;
		}

		int returnCode = pclose(pipe);

		if (returnCode != 0 || decryptedContent.empty())
		{
			throw std::runtime_error("Failed to decrypt secrets file. Ensure SOPS is configured properly.");
		}

		Json::CharReaderBuilder builder;
		Json::Value config;

		std::string errs;
		std::istringstream stream(decryptedContent);

		if (!Json::parseFromStream(builder, stream, &config, &errs))
		{
			throw std::runtime_error("Error parsing decrypted JSON: " + errs);
		}

		return config["api_key"].asString();
	}

	std::string getOrPromptForAPIKey()
	{
		const std::string keyFilePath = "./config/vars/.key";

		std::ifstream keyFile(keyFilePath);
		std::string apiKey;

		if (keyFile.is_open())
		{
			std::getline(keyFile, apiKey);

			keyFile.close();
		}

		if (apiKey.empty())
		{
			std::cout << "No API key found. Please enter your API key: ";

			std::getline(std::cin, apiKey);

			std::ofstream outFile(keyFilePath);

			if (outFile.is_open())
			{
				outFile << apiKey;
				outFile.close();

				std::cout << "API key stored securely in " << keyFilePath << "\n";
			}
			else
			{
				throw std::runtime_error("Failed to write API key to file.");
			}
		}

		if (!Utilities::validateAPIKey(apiKey))
		{
			throw std::runtime_error("Invalid API key. Please check your key and try again.");
		}

		return apiKey;
	}

	std::unordered_map<std::string, std::string> loadEnvironment()
	{
		char cwd[PATH_MAX];

		if (getcwd(cwd, sizeof(cwd)) == nullptr)
		{
			throw std::runtime_error("Failed to get current working directory.");
		}

		std::string envFilePath = std::string(cwd) + "/config/var/.env";

		return parseEnvFile(envFilePath);
	}

	std::unordered_map<std::string, std::string> parseEnvFile(const std::string &envFilePath)
	{
		std::unordered_map<std::string, std::string> envMap;
		std::ifstream envFile(envFilePath);

		if (!envFile.is_open())
		{
			throw std::runtime_error("Could not open .env file: " + envFilePath);
		}

		std::string line;

		while (std::getline(envFile, line))
		{
			if (line.empty() || line[0] == '#')
				continue;

			std::istringstream keyValuePair(line);
			std::string key, value;

			if (std::getline(keyValuePair, key, '=') && std::getline(keyValuePair, value))
			{
				envMap[key] = value;
			}
		}

		envFile.close();

		return envMap;
	}
}
