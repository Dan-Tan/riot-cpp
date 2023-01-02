#include "../../client.h"
#include <string>

using namespace client;

Json::Value RiotApiClient::CHALLENGES_V1_config(std::string region) {
    std::string end_url = "/lol/challenges/v1/challenges/config";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CHALLENGES_V1_percentiles(std::string region) {
    std::string end_url = "/lol/challenges/v1/challenges/percentiles";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CHALLENGES_V1_challenge_config(std::string challenge_id, std::string region) {
    challenge_id = encode_url(challenge_id);
    std::string end_url = "/lol/challenges/v1/challenges/" + challenge_id + "/config";
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

Json::Value RiotApiClient::CHALLENGES_V1_level(std::string challenge_id, std::string level, std::string region) {
    challenge_id = encode_url(challenge_id); level = encode_url(level);
    std::string end_url = "/lol/challenges/v1/challenges/" + challenge_id +"/leaderboards/by-level/" + level;
    end_url = this->get_BASE_URL(region) + end_url;
    return this->get(end_url, nullptr);
}

