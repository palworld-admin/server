#include <thread>
#include <admin/server.h>
#include <admin/callbacks.h>
#include <curl/curl.h>


admin::Server::Server(palworld::Sdk* sdk, ConfigFile* config)
{
	this->sdk    = sdk;
	this->config = config;

#ifdef ENABLE_RPC_SERVER
	// Initialize the RPC server
	const std::string host = config->json["server"]["host"];
	const uint16_t port    = config->json["server"]["port"];
	this->srv = new rpc::server(host, port);
#endif

	// Initialize libcurl
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

admin::Server::~Server()
{
	delete sdk;
	delete config;

#ifdef ENABLE_RPC_SERVER
	srv->close_sessions();
	srv->stop();
#endif

	// Cleanup libcurl	
	curl_global_cleanup();
}

void admin::Server::init() const
{
	// Register RPC callbacks
	register_callbacks();
}

void admin::Server::register_callbacks() const
{
	const auto d = new callbacks::Chat(config);
	palworld::Sdk::add_chat_callback(d);
}

void admin::Server::run() const
{
#ifdef ENABLE_RPC_SERVER
	// wait for 5 seconds to allow the game to start to avoid a crash
	LOG("Starting Server");
	std::this_thread::sleep_for(std::chrono::seconds(5));
	srv->async_run();
	LOG("Server Started!");
#endif
}
