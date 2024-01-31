#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <random>

class ConfigFile
{
public:
	/**
	 * \brief Loads a config file
	 * \param filepath File Absolute Directory
	 */
	explicit ConfigFile(const std::string& filepath);

	/**
	 * \brief saves the config file
	 * \return save success
	 */
	bool save() const;


	/**
	 * \brief checks if the config file is valid
	 * \return is file valid
	 */
	bool valid() const;

	/**
	 * \brief Generate Server Key
	 * \param length generated key length
	 * \return string
	 */
	static std::string generate_key(int length);
	nlohmann::json json;

private:
	std::string m_filepath;
};

inline ConfigFile::ConfigFile(const std::string& filepath)
{
	m_filepath = filepath;
	if (valid())
	{
		std::ifstream f(m_filepath);
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
	std::ofstream f(m_filepath);
	f << json;
	f.close();

	if (f.is_open())
		return false;

	return true;
}

inline bool ConfigFile::valid() const
{
	return std::filesystem::exists(m_filepath);
}

inline std::string ConfigFile::generate_key(const int length)
{
	std::string str(_("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
	std::random_device rd;
	std::mt19937 generator(rd());
	std::ranges::shuffle(str, generator);
	return str.substr(0, length);
}
