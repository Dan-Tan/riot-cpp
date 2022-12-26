#include <string>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include "client.h"

#define DEBUG 1

#include <fstream>
#include <iostream>

using namespace client;

RiotApiClient::RiotApiClient(std::string path_to_config, std::string path_to_log) {
    curl_global_init(CURL_GLOBAL_ALL);

    this->attempts = 5;

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
        this->header = curl_slist_append(header, (std::string("X-RIOT-TOKEN: ") + api_key).c_str());
    }
    
    this->log_path = path_to_log;
    this->easy_handle = curl_easy_init();
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
    int len = address.length();
    std::string res_;

    const char* encoded_address = curl_easy_escape(this->easy_handle, address.c_str(), len);

    curl_easy_setopt(this->easy_handle, CURLOPT_URL, encoded_address);
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);
    curl_easy_setopt(this->easy_handle, CURLOPT_VERBOSE, DEBUG);

    res_ = curl_easy_perform(easy_handle);
    bool json_conversion = reader.parse(res_, result);

    if (!json_conversion) {
        std::domain_error("Unable to cast output string to Json");
    }

    long response_code = 0;
    curl_easy_getinfo(this->handle, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code == 429) {
       if (attempt == 0) {
           this->handle_rate(true);
           attempt++;
           return this->get(end_url, region, attempt);
       } else if (attempt > this->n_attempts) {
           std::cout << "Aborting query, too many rate denials"
           return NULL;
       } else {
           this->handle_rate(false);
           attempt++;
           return this->get(end_url, region, attempt);
    }

    return result;
}

// LEAGUE V4 Get requests


Json::Value RiotApiClient::league_LEAGUE_V4(std::string queue, std::string league, std::string region) {
    std::string end_url = "/lol/league/v4/" + league + "/by-queue/" + queue;
    return this->get(end_url, region);
}

Json::Value RiotApiClient::player_LEAGUE_V4(std::string summoner_id, std::string region) {
    std::string end_url = "/lol/league/v4/entries/by-summoner/" + summoner_id;
    return this->get(end_url, region);
}


Json::Value RiotApiClient::MATCH_V5(std::string match_id, int type, std::string region) {
    if ((type > 2) || (type < 0)) {
        std::domain_error(std::string("invalid MATCH_V5 query type"));
    }
    std::string ending;
    if (type == 0) {
        ending = "by-puuid/" + match_id + "/ids";
    }
    else if (type == 1) {
        ending = match_id;
    }
    else {
        ending = match_id + "/timeline";
    }
    std::string end_url = "/lol/match/v5/matches/" + ending;
    return this->get(end_url, region);
}


Json::Value RiotApiClient::SUMMONER_V4(std::string ending, std::string region) {
    std::string end_url =  "/lol/summoner/v4/summoners/" + ending;
    return this->get(end_url, region);
}

void RiotApiClient::handle_rate(bool short) {
    if (!short) {
        std::cout << "Long timeout: waiting..." << std::endl;
        sleep(120);
    } else {
        std::cout << "waiting..." << std::endl;
        sleep(1);
    }
    std::cout << "trying again..." << std::endl;
    return;
}

bool RiotApiClient::handle_response(long response_code, bool log) {
    
}
