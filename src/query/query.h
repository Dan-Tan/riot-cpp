#pragma once
#include <concepts>
#include <jsoncpp/json/json.h>
#include <ctime>
#include <string>
#include <vector>
#include <functional>

namespace query {

    typedef struct query {
        std::string method_key;
        std::string routing_value;
        std::string url;
        std::time_t send_time = 0;
        Json::Value response_content;
        Json::Value response_header;
        long last_response = 0;
        int server_errors = 0;
    } query;

    template <typename T>
    concept param = requires {std::convertible_to<T, std::string>;};

    typedef struct Endpoint {

        std::shared_ptr<std::function<Json::Value(std::shared_ptr<query>)>> _query;
        std::string _url;
        
        private: 
            template <param R>
            std::stringstream construct_base(const R& routing) const; 

            template <std::size_t N, param ... Params>
            std::string full_query(const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& params) const;
        
            template <std::size_t N, param ... Params, param ... opts>
            std::string full_query(const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& params, const std::tuple<std::pair<std::string, opts>...>& optional_args) const;
            
        public:
            template <param R>
            std::shared_ptr<query> request(const std::string& key, const std::string& method_url, const R& routing);

            template <std::size_t N, param ... Params>
            std::shared_ptr<query> request(const std::string& key, const std::array<std::string, N>& method_urls, const Params& ... params);

            template <std::size_t N, param ... Params, param ... opts>
            std::shared_ptr<query> request(const std::string& key, const std::array<std::string, N>& method_urls, const Params& ... params, const std::pair<std::string, opts>& ... optional_args);

    } Endpoint;

    typedef struct ACCOUNT_V1 : public Endpoint{
        ACCOUNT_V1(const std::string url, std::shared_ptr<std::function<Json::Value(std::shared_ptr<query>)>> client_query) {
            _query = client_query;
            _url = url;
        }
        Json::Value by_puuid(const std::string& routing, const std::string& puuid);

        Json::Value by_riot_id(const std::string& routing, const std::string& gameName, const std::string& tagline);

        Json::Value by_game(const std::string& routing, const std::string& game, const std::string& puuid);
    } ACCOUNT_V1;

}
