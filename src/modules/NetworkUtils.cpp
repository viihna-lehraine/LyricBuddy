// File: src/modules/NetworkUtils.cpp

#include "../headers/NetworkUtils.h"

namespace NetworkUtils
{
	size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
	{
		size_t totalSize = size * nmemb;

		output->append((char *)contents, totalSize);

		return totalSize;
	}

	std::string callOpenAI(const std::string &prompt, const std::string &apiUrl, const std::string &apiKey, int maxTokens, const std::string &model, double temperature)
	{
		CURL *curl = curl_easy_init();

		if (!curl)
		{
			throw std::runtime_error("Failed to initialize cURL.");
		}

		std::string readBuffer;

		struct curl_slist *headers = NULL;

		headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
		headers = curl_slist_append(headers, "Content-Type: application/json");

		Json::Value payload;
		payload["model"] = model;
		payload["messages"] = Json::arrayValue;

		Json::Value systemMessage;
		systemMessage["role"] = "system";
		systemMessage["content"] = "You're my songwriting partner! But I have trouble writing lyrics. Please help me turn scraps into lyric ideas, or give me a good starting point, please!";

		Json::Value userMessage;
		userMessage["role"] = "user";
		userMessage["content"] = prompt;

		payload["messages"].append(systemMessage);
		payload["messages"].append(userMessage);

		payload["max_tokens"] = maxTokens;
		payload["temperature"] = temperature;

		Json::StreamWriterBuilder writer;
		std::string requestBody = Json::writeString(writer, payload);

		curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		CURLcode res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		return readBuffer;
	}

	std::string parseAIResponse(const std::string &response)
	{
		Json::CharReaderBuilder builder;
		Json::Value jsonResponse;

		std::string errs;
		std::istringstream stream(response);

		if (Json::parseFromStream(builder, stream, &jsonResponse, &errs))
		{
			return jsonResponse["choices"][0]["message"]["content"].asString();
		}
		else
		{
			throw std::runtime_error("Failed to parse AI response JSON: " + errs);
		}
	}
}
