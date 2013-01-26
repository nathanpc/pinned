/**
 *  request.h
 *
 *  @author Nathan Campos
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <cstdio>
#include <string>
#include <curl/curl.h>

class Request {
	private:
		std::string pinboard_server;
		std::string auth_token;

	public:
		Request();

		static void authenticate(std::string username, std::string password)
		void set_api_token(std::string _auth_token);

		static std::string raw_get(std::string url);
		std::string get(std::string params);
};

#endif