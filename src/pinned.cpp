/**
 *  pinned.cpp
 *  Pinned - Pinboard client for hackers.
 *
 *  @author Nathan Campos
 */
 
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <json/json.h>

#include "request.h"
#include "config.h"
#include "color.h"
using namespace std;

Config config;
Request request;

void handle_arguments(int argc, char *argv[]) {
	if (argc > 1) {
		if (strcmp(argv[1], "--auth") == 0) {
			// Check if all the arguments were passed.
			if (argc < 3) {
				cerr << "Usage: pinned --auth <username> <password>" << endl;
				exit(1);
			}

			Request::authenticate(string(argv[2]), string(argv[3]));
		} else if (strcmp(argv[1], "--all") == 0) {
			// List all the posts.
			request.set_auth_token(config.load_auth_token());
			request.list_posts();
		}
	} else {
		// No arguments.
		request.set_auth_token(config.load_auth_token());
		request.list_posts();
	}
}

int main(int argc, char *argv[]) {
	handle_arguments(argc, argv);
	
	return 0;
}