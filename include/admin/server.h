#pragma once
#include <palworld/sdk.h>
#include <rpc/server.h>

namespace admin
{
	class Server
	{
	public:
		Server(palworld::Sdk* sdk, ConfigFile* config);
		~Server();

		void init() const;

		void register_callbacks() const;
		void run() const;

	private:
		palworld::Sdk* sdk;
		ConfigFile* config;
		rpc::server* srv;
	};
}
