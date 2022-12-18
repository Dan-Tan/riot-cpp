#include <string.h>
#include <jsoncpp/json/json.h>
#include "client.h"

using namespace client 

RiotApiClient::RiotApiClient(std::string api_key) {
    curl_global_init(CURL_GLOBAL_ALL);
    header = curl_slist_append(header, "X-RIOT-TOKEN: " + api_key);
    easy_handle = curl_easy_init();
}

RiotApiClient::~RiotApiClient() {
    curl_global_cleanup();
}

std::string RiotApiClient::get_BASE_URL(std::string region) {
    return BASE_URL_START + region + BASE_URL_END;
}

std::string RiotApiClient::get(std::string end_url, std::string region) {

    std::string = this.get_BASE_URL(region)

    curl_easy_setopt(easy_handle, CURLOPT_URL, address.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, header);

    res_ = curl_easy_perform();

    return res_
}


