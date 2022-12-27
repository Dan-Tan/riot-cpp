#include <string>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include "client.h"
#include <chrono>
#include <thread>

#define DEBUG 1

#include <fstream>
#include <iostream>

using namespace client;

RiotApiClient::RiotApiClient(std::string path_to_config, std::string path_to_log) {
    curl_global_init(CURL_GLOBAL_ALL);

    this->n_attempts = 5;

    std::ifstream config(path_to_config);
    Json::Reader reader;

    if (!config) {
        std::string err_msg("Cannot open config file: ");
        err_msg = err_msg + path_to_config;
        throw std::domain_error(err_msg);
    }
    
    Json::Value root;
    bool retrieving_key = reader.parse(config, root);

    if (!retrieving_key) {
        std::string err_msg("Unable to read api json");
        throw std::domain_error(err_msg);
    }
    else {
        std::string api_key = root["api-key"].asString();
        std::cout << "Retrieved API key" << std::endl;
        this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
    }
    
    this->path_to_log = path_to_log;
    this->easy_handle = curl_easy_init();

    std::cout << "Client initialised" << std::endl;
}

RiotApiClient::~RiotApiClient() {
    curl_global_cleanup();
}

std::string RiotApiClient::get_BASE_URL(std::string region) {
    return BASE_URL_START + region + BASE_URL_END;
}

Json::Value RiotApiClient::get(std::string end_url, std::string region, int attempt) {

    Json::Reader reader;
    Json::Value result;

    // construct query url
    std::string address = this->get_BASE_URL(region) + end_url; 
    std::cout << address << std::endl;
    int len = address.length();
    std::string res_;

    const char* encoded_address = curl_easy_escape(this->easy_handle, address.c_str(), len);

    curl_easy_setopt(this->easy_handle, CURLOPT_URL, encoded_address);
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);
    curl_easy_setopt(this->easy_handle, CURLOPT_VERBOSE, DEBUG);

    res_ = curl_easy_perform(this->easy_handle);
    std::cout << res_ << std::endl;
    bool json_conversion = reader.parse(res_, result);

    if (!json_conversion) {
        std::domain_error("Unable to cast output string to Json");
    }

    long response_code = 0;
    curl_easy_getinfo(this->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code == 429) {
        if (attempt == 0) {
           this->handle_rate(true);
           attempt++;
           return this->get(end_url, region, attempt);
        } else if (attempt > this->n_attempts) {
           std::cout << "Aborting query, too many rate denials" << std::endl;
           return result;
        } else {
           this->handle_rate(false);
           attempt++;
           return this->get(end_url, region, attempt);
        }
    }

    std::cout << std::to_string(response_code) << std::endl;
    return result;
}



void RiotApiClient::handle_rate(bool wait_type) {
    if (!wait_type) {
        std::cout << "Long timeout: waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(120));
    } else {
        std::cout << "waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "trying again..." << std::endl;
    return;
}

bool RiotApiClient::handle_response(long response_code, bool log) {
    if (log) {
        return true;
    } else {
        return (response_code == 0);
    }
}
