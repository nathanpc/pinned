/**
 *  request.h
 *
 *  @author Nathan Campos
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <cstdio>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>

class Request {
	private:
		std::string pinboard_server;
		std::string auth_token;

	public:
		Request();

		static void authenticate(std::string username, std::string password);
		void set_auth_token(std::string _auth_token);

		static std::string raw_get(std::string url);
		std::string get(std::string path);
		std::string get(std::string path, std::vector<std::string> params);

		static Json::Value parse_json(std::string response);
		static void urlencode_vector(std::vector<std::string> &params);
		
		void list_posts();
		void add_post(int argc, char *argv[]);
};

#endif