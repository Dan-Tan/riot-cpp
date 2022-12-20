#include <string>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include "client.h"

#include <fstream>
#include <iostream>

using namespace client;

RiotApiClient::RiotApiClient(std::string path_to_config) {
    curl_global_init(CURL_GLOBAL_ALL);

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

    this->easy_handle = curl_easy_init();
}

RiotApiClient::~RiotApiClient() {
    curl_global_cleanup();
}

std::string RiotApiClient::get_BASE_URL(std::string region) {
    return BASE_URL_START + region + BASE_URL_END;
}

std::string RiotApiClient::get(std::string end_url, std::string region) {

    std::string address = this->get_BASE_URL(region) + end_url; 
    std::string res_;

    std::cout << address << std::endl;

    curl_easy_setopt(this->easy_handle, CURLOPT_URL, address.c_str());
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(this->easy_handle, CURLOPT_HTTPHEADER, this->header);
    curl_easy_setopt(this->easy_handle, CURLOPT_VERBOSE, 1);
    std::cout << "Sending request" << std::endl;
    res_ = curl_easy_perform(easy_handle);

    return res_;
}

// LEAGUE V4 Get requests


std::string RiotApiClient::league_LEAGUE_V4(std::string queue, std::string league, std::string region) {
    std::string end_url = "/lol/league/v4/" + league + "/by-queue/" + queue;
    return this->get(end_url, region);
}
std::string RiotApiClient::player_LEAGUE_V4(std::string summoner_id, std::string region) {
    std::string end_url = "/lol/league/v4/entries/by-summoner/" + summoner_id;
    return this->get(end_url, region);
}


std::string RiotApiClient::MATCH_V5(std::string match_id, int type, std::string region) {
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


std::string RiotApiClient::SUMMONER_V4(std::string ending, std::string region) {
    std::string end_url =  "/lol/summoner/v4/summoners/" + ending;
    return this->get(end_url, region);
}
int main() {

    std::string config_path = "../../.api_keys/riot_config.json";
    RiotApiClient test_client(config_path); 
    std::string region = "oc1";
    std::string query = "by-name/Leapo";
    std::string output = test_client.SUMMONER_V4(query, region);
    std::cout << output << std::endl;
}
