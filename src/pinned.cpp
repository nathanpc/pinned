/**
 *  pinned.cpp
 *  Pinned - Pinboard client for hackers.
 *
 *  @author Nathan Campos
 */
 
#include <iostream>
#include <string>
#include <json/json.h>

using namespace std;

int main(int argc, char *argv[]) {
    string json = "{\"test\": \"testing\"}";
    Json::Value root;
    Json::Reader reader;
    
    bool parsingSuccessful = reader.parse(json, root);
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        return 1;
    }
    return 0;
}