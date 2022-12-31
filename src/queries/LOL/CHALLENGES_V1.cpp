#include "../../client.h"
#include <string>
#define ENCODE(x) (std::string(curl_easy_escape(this->easy_handle, x.c_str(), x.length())))

Json::Value RiotApiClient::CHALLENGES_V1_config(std::string region) {
    std::string end_url = "/lol/challenges/v1/challenges/config";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CHALLENGES_V1_percentiles(std::string region) {
    std::string end_url = "/lol/challenges/v1/challenges/percentiles";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CHALLENGES_V1_challenge_config(std::string challenge_id, std::string region) {
    challenge_id = ENCODE(challenge_id);
    std::string end_url = "/lol/challenges/v1/challenges/" + challenge_id + "/config";
    return this->get(end_url, region, NULL);
}

Json::Value RiotApiClient::CHALLENGES_V1_level(std::string challenge_id, std::string level, std::string region) {
    challenge_id = ENCODE(challenge_id); level = ENCODE(level);
    std::string end_url = "/lol/challenges/v1/challenges/" + challenge_id +"/leaderboards/by-level/" + level;
    return this->get(end_url, region, NULL);
}

