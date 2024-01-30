#include <palworld/sdk.h>
#include <admin/callbacks.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

void admin::callbacks::Chat::call(long long a1, palworld::FPalChatMessage* chat_message)
{
	// Create a CURL handle
	if (!config_file->json["discord"]["webhook_url"].empty() &&
		chat_message->Category <= 0 ||
		chat_message->Category == 1)
	{
		CURL* curl = curl_easy_init();

		// Set the target URL
		const std::string webhook_url = config_file->json[_("discord")][_("webhook_url")];
		curl_easy_setopt(curl,
		                 CURLOPT_URL,
		                 webhook_url.c_str());

		// Set the HTTP header for content type
		curl_slist* headers = nullptr;
		headers             = curl_slist_append(headers, _("Content-Type: application/json"));
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Set the POST data
		nlohmann::json data;
		data[_("username")]   = _("Palworld");
		data[_("avatar_url")] = _("https://i.imgur.com/VlmFzXD.png");
		if (!config_file->json[_("discord")][_("bot_name")].empty())
		{
			data[_("username")] = config_file->json[_("discord")][_("bot_name")];
		}

		if (!config_file->json[_("discord")][_("bot_avatar")].empty())
		{
			data[_("avatar_url")] = config_file->json[_("discord")][_("bot_avatar")];
		}

		data[_("embeds")][0][_("author")][_("name")] = chat_message->Sender.c_str();
		data[_("embeds")][0][_("description")]       = chat_message->Message.c_str();
		data[_("embeds")][0][_("color")]             = 0x00FF00;

		if (!config_file->json[_("discord")][_("color")].empty())
		{
			data[_("embeds")][0]["color"] = config_file->json[_("discord")][_("color")];
		}

		const std::string jsonstr = data.dump();
		LOG("RAW JSON: %s", jsonstr.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(jsonstr.size()));
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstr.c_str());

		// Perform the HTTP POST request
		curl_easy_perform(curl);

		// Cleanup
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);

		data.clear();
	}

	LOG("[CALLBACK] sender: %s, message: %s type: %d uid: %s",
	    chat_message->Sender.c_str(),
	    chat_message->Message.c_str(),
	    chat_message->Category,
	    chat_message->SenderPlayerUId.to_string().c_str()
	);
}
