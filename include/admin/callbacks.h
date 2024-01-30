#pragma once
#include <palworld/sdk.h>

namespace admin::callbacks
{
	class Chat : virtual public palworld::HookCallback
	{
	public:
		explicit Chat(ConfigFile* config_file)
		{
			this->config_file = config_file;
		}

		/**
		 * \brief callback function for chat messages
		 * \param a1 idk maybe a pointer an UObject?
		 * \param chat_message struct containing the message
		 */
		void call(long long a1, palworld::FPalChatMessage* chat_message) override;

	private:
		ConfigFile* config_file = nullptr;
	};
}
