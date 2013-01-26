/**
 *  request.cpp
 *
 *  @author Nathan Campos
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <curl/curl.h>

#include "request.h"
#include "config.h"
using namespace std;

Request::Request() {
	pinboard_server = "https://api.pinboard.in/v1";
}

void Request::set_api_token(string _auth_token) {
	auth_token = _auth_token;
}

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
	((string*)stream)->append((char*)ptr, 0, size * count);
	return size * count;
}

string Request::raw_get(string url) {
	CURL *curl;
	CURLcode res;
	string response;

	curl = curl_easy_init();
	if (curl) {
		// Setup cURL.
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// Perform request.
		res = curl_easy_perform(curl);

		// Check for errors.
		if (res != CURLE_OK) {
			cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
		}

		// Cleanup cURL shit.
		curl_easy_cleanup(curl);
	}

	return response;
}

string Request::get(string params) {
	return raw_get(pinboard_server + params + "?auth_token=" + auth_token);
}

void Request::authenticate(string username, string password) {
	Config config;
	string response = Request::raw_get("https://" + username + ":" + password + "@api.pinboard.in/v1/user/api_token/?format=json");

	// Check you actually got authenticated.
	if (response == "401 Forbidden") {
		cerr << RED << "Username or password is incorrect. :(" << RESET << endl;
		exit(1);
	}

	// Parse the response JSON.
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(response, root)) {
		// Save the authentication token to a file.
		string auth_token = root.get("result", "").asString();
		if (!auth_token.empty()) {
			config.save_auth_token(username, auth_token);
			cout << "Authentication token saved to ~/.pinboard_token" << endl;
		} else {
			cerr << RED << "Couldn't get the authentication token from the JSON response." << RESET << endl;
			exit(1);
		}
	} else {
		cout << "Server Response: " << response << endl;
		cerr << RED << "Something went wrong while trying to parse the response from the server:" << RESET << endl;
		cerr << reader.getFormattedErrorMessages() << endl;

		exit(1);
	}
}