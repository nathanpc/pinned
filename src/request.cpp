/**
 *  request.cpp
 *
 *  @author Nathan Campos
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <json/json.h>

#include "base64/base64.h"
#include "libinet/uri.h"
#include "libinet/http.h"

#include "request.h"
#include "config.h"
#include "color.h"
using namespace std;

Request::Request() {
	pinboard_server = "https://api.pinboard.in/v1";
}

void Request::set_auth_token(string _auth_token) {
	auth_token = _auth_token;
}

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
	((string*)stream)->append((char*)ptr, 0, size * count);
	return size * count;
}

string Request::raw_get(string url, vector<vector<string> > headers) {
	URI uri(url);
	HTTP http(uri);
    HTTP_Response response;

    http.add_header("User-Agent", "Pinned/0.2");
    for (size_t i = 0; i < headers.size(); i++) {
    	http.add_header(headers.at(i).at(0), headers.at(i).at(1));
    }

    response = http.request("GET");
	return response.body;
}

string Request::raw_get(string url) {
	vector<vector<string> > headers;
	return raw_get(url, headers);
}

string Request::get(string path) {
	return raw_get(pinboard_server + path + "?auth_token=" + auth_token + "&format=json");
}

string Request::get(string path, vector<string> params) {
	string final_url = pinboard_server + path + "?auth_token=" + auth_token + "&format=json";

	urlencode_vector(params);
	for (size_t i = 0; i < params.size(); i++) {
		final_url += "&";
		final_url += params.at(i);
	}

	return raw_get(final_url);
}

Json::Value Request::parse_json(string response) {
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(response, root)) {
		return root;
	} else {
		cout << "Server Response: " << response << endl;
		cerr << RED << "Something went wrong while trying to parse the response from the server:" << RESET << endl;
		cerr << reader.getFormattedErrorMessages() << endl;

		exit(1);
	}
}

void Request::authenticate(string username, string password) {
	Config config;
	vector<vector<string> > header;
	vector<string> auth_header;
	string auth_str = username + ":" + password;

	auth_header.push_back("Authorization");
	auth_header.push_back("Basic " + string(Base64::encode(auth_str.c_str(), strlen(auth_str.c_str()))));

	header.push_back(auth_header);
	string response = Request::raw_get("https://api.pinboard.in/v1/user/api_token/?format=json", header);

	// Check you actually got authenticated.
	if (response == "401 Forbidden") {
		cerr << RED << "Username or password is incorrect. :(" << RESET << endl;
		exit(1);
	}

	// Parse the response JSON.
	Json::Value json = parse_json(response);
	// Save the authentication token to a file.
	string auth_token = json.get("result", "").asString();
	if (!auth_token.empty()) {
		config.save_auth_token(username, auth_token);
		cout << "Authentication token saved to ~/.pinboard_token" << endl;
	} else {
		cerr << RED << "Couldn't get the authentication token from the JSON response." << RESET << endl;
		exit(1);
	}
}

void Request::authenticate(string _auth_token) {
	Config config;
	string username = _auth_token.substr(0, _auth_token.find(":"));
	string token = _auth_token.substr(_auth_token.find(":") + 1);

	config.save_auth_token(username, token);
	cout << "Authentication token saved to ~/.pinboard_token" << endl;
}

void Request::list_posts(int limit) {
	string posts;

	if (limit) {
		stringstream tmp;
		tmp << "results=" << limit;

		vector<string> params;
		params.push_back(tmp.str());

		posts = get("/posts/all", params);
	} else {
		posts = get("/posts/all");
	}

	// Parse the response JSON.
	Json::Value json = parse_json(posts);
	for (unsigned int i = 0; i < json.size(); i++) {
		Json::Value post = json[i];

		// Display title.
		cout << i + 1 << ". ";
		if (post["toread"].asString() == "yes") {
			cout << BOLDRED;
		} else {
			cout << BOLD;
		}
		cout << post["description"].asString() << RESET << endl;

		// Display description text.
		string extended = post.get("extended", "").asString();
		if (!extended.empty()) {
			cout << "    " << extended << endl;
		}

		// Display URL.
		cout << "    " << CYAN << UNDERSCORE << post["href"].asString() << RESET << endl;

		if (i + 1 != json.size()) {
			cout << endl;
		}
	}
}

void Request::add_post(int argc, char *argv[]) {
	vector<string> param_keys;
	param_keys.push_back("url=");
	param_keys.push_back("description=");
	param_keys.push_back("extended=");
	param_keys.push_back("tags=");

	vector<string> params;
	for (size_t i = 0; i < (argc - 2); i++) {
		string final_param = param_keys.at(i);
		final_param += argv[i + 2];

		params.push_back(final_param);
	}

	string response = get("/posts/add", params);
	if (parse_json(response)["result_code"].asString() == "done" ) {
		cout << GREEN << "Bookmarked" << RESET << endl;
	} else {
		cout << RED << "Something went wrong" << RESET << endl;
	}
}

void Request::delete_post(const char *url) {
	vector<string> params;
	params.push_back(string("url=") + url);

	string response = get("/posts/delete", params);
	if (parse_json(response)["result_code"].asString() == "done" ) {
		cout << GREEN << "Bookmark deleted" << RESET << endl;
	} else {
		cout << RED << "Something went wrong" << RESET << endl;
 	}
}

void str_replace(string &str, const string &from, const string &to) {
	if (from.empty()) {
		return;
	}

	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void Request::urlencode_vector(vector<string> &params) {
	for (size_t i = 0; i < params.size(); i++) {
		str_replace(params.at(i), " ", "+");
	}
}
