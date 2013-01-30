/**
 *  pinned.cpp
 *  Pinned - Pinboard client for hackers.
 *
 *  @author Nathan Campos
 */

#include <iostream>
#include <vector>
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

void print_usage() {
	cout << BOLD << "Usage: " << RESET << "pinned [--auth | --all | --add | --delete | --help] [params]" << endl << endl;

	cout << BOLD << "Commands:" << RESET << endl;
	cout << "    --auth \t Authenticate with Pinboard" << endl;
	cout << "    --all \t List all your bookmarks" << endl;
	cout << "    --add \t Add a new bookmark" << endl;
	cout << "    --delete \t Delete a bookmark" << endl;
	cout << "    --help \t This" << endl;
}

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

			if (argc == 2) {
				request.list_posts();
			} else {
				request.list_posts(atoi(argv[2]));
			}
		} else if (strcmp(argv[1], "--add") == 0) {
			if (argc < 3) {
				cerr << "Usage: pinned --add <url> <title> [description] [tags]";
				exit(1);
			}

			request.set_auth_token(config.load_auth_token());
			request.add_post(argc, argv);
		} else if (strcmp(argv[1], "--delete") == 0) {
			if (argc < 2) {
				cerr << "Usage: pinned --delete <url>";
				exit(1);
			}

			request.set_auth_token(config.load_auth_token());
			request.delete_post(argv[2]);
		} else if (strcmp(argv[1], "--help") == 0) {
			print_usage();
		} else {
			cerr << RED << "Unknown argument: " << argv[1] << RESET << endl;
			print_usage();
			exit(1);
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
