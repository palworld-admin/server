#include <thread>
#include <palworld/sdk.h>
#include <admin/server.h>

/**
 * \brief This function releases the module and exits the thread
 * \param module 
 */
void release(const HMODULE module)
{
	ue::utils::disable_console();
	FreeLibraryAndExitThread(module, 0);
}

/**
 * \brief this function generates a default configuration file
 * \param config ConfigFile instance
 */
void generate_default_config(ConfigFile& config)
{
	ue::utils::log(_("Configuration file is invalid. Generating default configuration"));
	config.json[_("server")][_("host")] = _("127.0.0.1");
	config.json[_("server")][_("port")] = 5555;
	config.json[_("server")][_("key")]  = config.generate_key(8);
	if (config.save())
	{
		ue::utils::log(_("Configuration file saved!"));
	}
	else
	{
		ue::utils::log(_("Failed to generate configuration file!"));
	}
};


/**
 * \brief initializes the shared library
 * \param module game instance
 */
void initialize(const HMODULE module)
{
	ue::utils::enable_console();
	// Initialize messages
	ue::utils::print(_("  ____       _                    _     _    _       _           _       \n"));
	ue::utils::print(_(" |  _ \\ __ _| |_      _____  _ __| | __| |  / \\   __| |_ __ ___ (_)_ __  \n"));
	ue::utils::print(_(" | |_) / _` | \\ \\ /\\ / / _ \\| '__| |/ _` | / _ \\ / _` | '_ ` _ \\| | '_ \\ \n"));
	ue::utils::print(_(" |  __/ (_| | |\\ V  V / (_) | |  | | (_| |/ ___ \\ (_| | | | | | | | | | | \n"));
	ue::utils::print(_(" |_|   \\__,_|_| \\_/\\_/ \\___/|_|  |_|\\__,_/_/   \\_\\__,_|_| |_| |_|_|_| |_|\n"));
	ue::utils::print("\n");
	ue::utils::log(_("palworld-admin version: %s built on: %s"), "0.0.3", __TIMESTAMP__);
	ue::utils::log(_("Initializing..."));

	// Load config
	// Fixes: https://github.com/palworld-admin/server/issues/1
	char module_filename[1024]{'\0'};
	GetModuleFileNameA(module, module_filename, sizeof(module_filename) / sizeof(wchar_t));

	const auto file_path           = std::filesystem::path(module_filename).parent_path();
	const std::string abs_filepath = file_path.string() + _("\\palworld-admin.config.json");
	ue::utils::log("Configuration Location: %s", abs_filepath.c_str());

	ConfigFile config(_(abs_filepath));
	if (config.valid())
	{
		ue::utils::log(_("Configuration file loaded!"));

		// check if config is valid
		if (config.json[_("server")][_("host")].empty() ||
			config.json[_("server")][_("port")].empty() ||
			config.json[_("server")][_("key")].empty())
		{
			generate_default_config(config);
		}
	}
	else
	{
		// generate default config
		generate_default_config(config);
	}

	// needed for logging 
	const std::string host = config.json[_("server")][_("host")];
	const int32 port       = config.json[_("server")][_("port")];
	const std::string key  = config.json[_("server")][_("key")];

	ue::utils::log(_("[Configuration Information]"));
	ue::utils::log(_("Admin Server Host: %s"), host.c_str());
	ue::utils::log(_("Admin Server Port: %d"), port);
	ue::utils::log(_("Admin Server Key: %s"), key.c_str());

	if (!config.json[_("discord")].empty())
	{
		ue::utils::log(_("[Discord Information]"));
		if (!config.json[_("discord")][_("webhook_url")].empty())
		{
			ue::utils::log(_("Discord Webhook is Enabled"));
			const std::string webhook_url = config.json["discord"]["webhook_url"];
			ue::utils::log(_("Webhook URL: %s"), webhook_url.c_str());
		}
	}


	// Initialize SDK
	palworld::Sdk sdk(&config);
	ue::utils::log(_("SDK Initialized!"));

	// Initialize server
	admin::Server server(&sdk, &config);

	// Initialize server
	server.init();
	ue::utils::log(_("Server Initialized!"));

	// Initialize hooks
	palworld::hooks::init();

	// Run server
	server.run();

	LOG("You can unhook the DLL by pressing END key");
	while (!LI_FN(GetAsyncKeyState)(VK_END)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

	// remove hooks
	palworld::hooks::remove();

	// release thread
	release(module);
}

// ReSharper disable once CppInconsistentNaming
bool __stdcall DllMain(HMODULE module, const uint32_t call_reason, [[maybe_unused]] void* reserved)
{
	if (call_reason != DLL_PROCESS_ATTACH)
	{
		return true;
	}

	LI_FN(DisableThreadLibraryCalls)(module);
	if (const auto handle = LI_FN(CreateThread)(nullptr, NULL,
	                                            reinterpret_cast<unsigned long(__stdcall*)(void*)>(initialize),
	                                            module, NULL, nullptr))
	{
		LI_FN(CloseHandle)(handle);
	}

	return true;
}
