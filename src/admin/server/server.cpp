#include <thread>
#include <admin/server.h>
#include <admin/callbacks.h>
#include <curl/curl.h>


admin::Server::Server(palworld::Sdk* sdk, ConfigFile* config)
{
	this->sdk    = sdk;
	this->config = config;

	// Initialize the RPC server
	const std::string host = config->json["server"]["host"];
	const uint16_t port    = config->json["server"]["port"];
	this->srv              = new rpc::server(host, port);

	// Initialize libcurl
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

admin::Server::~Server()
{
	delete sdk;
	delete config;
	srv->close_sessions();
	srv->stop();

	// Cleanup libcurl	
	curl_global_cleanup();
}

void admin::Server::init() const
{
}

void admin::Server::register_callbacks() const
{
	const auto d = new callbacks::Chat(config);
	palworld::Sdk::add_chat_callback(d);
}

void admin::Server::run() const
{
#ifdef _DEBUG
	srv->run();
#endif
	for (;;)
	{
	}
}
