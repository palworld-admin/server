#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <random>

class ConfigFile
{
public:
	explicit ConfigFile(const std::string& filename);
	bool save() const;
	bool valid() const;
	static std::string generate_key(int length);
	nlohmann::json json;

private:
	std::string m_filename;
};

inline ConfigFile::ConfigFile(const std::string& filename)
{
	m_filename = filename;
	if (valid())
	{
		std::ifstream f(filename);
		if (f.is_open())
			json = nlohmann::json::parse(f);
	}
	else
	{
		// generate default config
		json = nlohmann::json::object();
	}
}

inline bool ConfigFile::save() const
{
	std::ofstream f(m_filename);
	f << json;
	f.close();

	if (f.is_open())
		return false;

	return true;
}

inline bool ConfigFile::valid() const
{
	return std::filesystem::exists(m_filename);
}

inline std::string ConfigFile::generate_key(const int length)
{
	std::string str(_("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
	std::random_device rd;
	std::mt19937 generator(rd());
	std::ranges::shuffle(str, generator);
	return str.substr(0, length);
}
