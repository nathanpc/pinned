/**
 *  config.cpp
 *
 *  @author Nathan Campos
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <wordexp.h>

#include "config.h"
using namespace std;

Config::Config() {
	file_location = expand_path("~/.pinboard_token");
}

string Config::expand_path(const char *directory) {
	wordexp_t exp_result;
	wordexp(directory, &exp_result, 0);

	return string(exp_result.we_wordv[0]);
}

void Config::save_auth_token(string username, string auth_token) {
	ofstream file(file_location.c_str());
	file << username << ":" << auth_token;
	file.close();
}

string Config::load_auth_token() {
	ifstream file(file_location.c_str());
	if (file.is_open()) {
		string auth_token;
		getline(file, auth_token);

		return auth_token;
	} else {
		cerr << "Authentication token not found. Please run pinned with the --auth argument" << endl;
		exit(1);
	}
}