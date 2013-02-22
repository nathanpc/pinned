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
	cout << "    -s (--auth)   \t Authenticate with Pinboard" << endl;
	cout << "    -a (--all)    \t List all your bookmarks" << endl;
	cout << "    -i (--add)    \t Add a new bookmark" << endl;
	cout << "    -d (--delete) \t Delete a bookmark" << endl;
	cout << "    -h (--help)   \t This" << endl;
}

bool check_arg(const char arg[], string short_arg, string long_arg) {
	return (strcmp(arg, short_arg.c_str()) == 0) || (strcmp(arg, long_arg.c_str()) == 0);
}

void handle_arguments(int argc, char *argv[]) {
	if (argc > 1) {
		if (check_arg(argv[1], "-s", "--auth")) {
			// Check if all the arguments were passed.
			if (argc < 3) {
				cerr << "Usage: pinned --auth <username> <password>" << endl;
				exit(1);
			}

			Request::authenticate(string(argv[2]), string(argv[3]));
		} else if (check_arg(argv[1], "-a", "--all")) {
			// List all the posts.
			request.set_auth_token(config.load_auth_token());

			if (argc == 2) {
				request.list_posts();
			} else {
				request.list_posts(atoi(argv[2]));
			}
		} else if (check_arg(argv[1], "-i", "--add")) {
			if (argc < 3) {
				cerr << "Usage: pinned --add <url> <title> [description] [tags]";
				exit(1);
			}

			request.set_auth_token(config.load_auth_token());
			request.add_post(argc, argv);
		} else if (check_arg(argv[1], "-d", "--delete")) {
			if (argc < 2) {
				cerr << "Usage: pinned --delete <url>";
				exit(1);
			}

			request.set_auth_token(config.load_auth_token());
			request.delete_post(argv[2]);
		} else if (check_arg(argv[1], "-h", "--help")) {
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
