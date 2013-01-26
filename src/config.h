/**
 *  config.cpp
 *
 *  @author Nathan Campos
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>

class Config {
	private:
		std::string file_location;

		std::string expand_path(const char *directory);

	public:
		Config();

		void save_auth_token(std::string username, std::string auth_token);
		std::string load_auth_token();
};

#endif