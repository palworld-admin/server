#pragma once
#include <palworld/sdk.h>

// Server Definition

// The RPC Server is disabled in release mode
// It should be enabled only for debugging purposes
// To enable it, remove _DEBUG
#ifdef _DEBUG
#define ENABLE_RPC_SERVER
#endif

#ifdef ENABLE_RPC_SERVER
#include <rpc/server.h>
#endif

namespace admin
{
	class Server
	{
	public:
		Server(palworld::Sdk* sdk, ConfigFile* config);
		~Server();

		/**
		 * \brief Initializes the server
		 */
		void init() const;

		/**
		 * \brief Registers the callbacks, such as chat, objects callbacks
		 */
		void register_callbacks() const;

		/**
		 * \brief Runs the RPC server
		 */
		void run() const;

	private:
		palworld::Sdk* sdk;
		ConfigFile* config;
#ifdef ENABLE_RPC_SERVER
		rpc::server* srv;
#endif
	};
}
