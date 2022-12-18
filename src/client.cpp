#include <string.h>
#include <jsoncpp/json/json.h>
#include "client.h"

using namespace client 

RiotApiClient::RiotApiClient(std::string keuy) {
    api_key = key
    easy_handle = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);
}

RiotApiClient::~RiotApiClient() {
    curl_global_cleanup();
}

std::string get_BASE_URL(std::string region) {
    return BASE_URL_START + region + BASE_URL_END;
}

std::string RiotApiClient::get(std::string end_url) {

    curl_easy_setopt(easy_handle, CURLOPT_URL, address.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_XOAUTH2_BEARER, api_key);
    curl_easy_setopt(_connection, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
}


