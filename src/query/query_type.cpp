#include <string>
#include <tuple>
#include <numeric>
#include <ctime>
#include <vector>
#include <utility>
#include <stdexcept>
#include "query.h"

namespace query {

template <param P>
static inline std::string construct_args(const std::stringstream& accumm, const std::pair<P, P>& optional_arg) {
    return accumm << optional_arg.first << "=" << optional_arg.second << "&";
}

template <param ... opts>
std::string query_construct(const std::tuple<std::pair<std::string, opts> ...>& optional_args){
    std::stringstream ss;
    ss = std::accumulate(optional_args.begin(), --optional_args.end(), std::stringstream("?"), construct_args);
    const std::size_t n_args = std::tuple_size<decltype(optional_args)>::value;
    ss << std::get<n_args-1>(optional_args).first << "=" << std::get<n_args-1>(optional_args).second;
    return ss.str();
}

template <param R>
std::stringstream Endpoint::construct_base(const R& routing) const {
    std::stringstream ss;
    ss << "https://" << routing << ".api.riotgames.com/" << this->_url;
    return ss;
}

template <param P>
static inline void full_helper(std::stringstream& accum, const std::string& url_seg, const P& param) {
    accum << url_seg << param;
}

template <std::size_t N, param ... Params, std::size_t ... Ixs>
static inline void full_iter(std::stringstream& accum, const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& params, std::index_sequence<Ixs ...>) {
    (full_helper(accum, method_urls[Ixs], std::get<Ixs+1>(params)), ...);
}

template <std::size_t N, param ... Params>
std::string Endpoint::full_query(const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& store) const {
    std::stringstream base_url = this->construct_base(std::get<0>(store));
    const std::size_t n_params = std::tuple_size<std::tuple<Params...>>::value - 1;
    full_iter(base_url, method_urls, store, std::make_index_sequence<N>{});
    if constexpr (n_params > N) {
        base_url << std::get<N>(store);
    }
    return base_url.str();
}

template <std::size_t N, param ... Params, param ... opts>
std::string Endpoint::full_query(const std::array<std::string, N>& method_urls, const std::tuple<Params ...>& params, const std::tuple<std::pair<std::string, opts>...>& optional_args) const {
    std::string final_url = this->full_query(method_urls, params);
    std::string optional = query_construct(optional_args);
    final_url += optional;
    return final_url;
}
template <param R>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::string& method_url, const R& routing) {
    std::string final_url = this->construct_base(routing).str();
    std::shared_ptr<query> request = std::make_shared<query>(key, routing, final_url);
    return {};
}

template <std::size_t N, param ... Params>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::array<std::string, N>& method_urls, const Params& ... params) {
    const std::tuple<Params ...> store(params ...);
    std::string final_url = this->full_query(method_urls, store);
    std::shared_ptr<query> request = std::make_shared<query>(key, std::get<0>(store), final_url);
    return request;
}

template <std::size_t N, param ... Params, param ... opts>
std::shared_ptr<query> Endpoint::request(const std::string& key, const std::array<std::string, N>& method_urls, const Params& ... params, const std::pair<std::string, opts>& ... optional_args) {
    std::tuple<Params ...> store_params(params ...);
    std::tuple<std::pair<std::string, opts> ...> store_opts(optional_args...);
    std::string final_url = this->full_query(method_urls, store_params, store_opts);
    std::shared_ptr<query> request = std::make_shared<query>(key, std::get<0>(store_params), final_url);
    return request;
}

Json::Value ACCOUNT_V1::by_puuid(const std::string& routing, const std::string& puuid) {
    const std::string method_key = "ACCOUNT-V1-by-puuid";
    const std::array<std::string, 1> method_urls= {"accounts/by-puuid/"};
    std::shared_ptr<query> new_request = request(method_key, method_urls, routing, puuid);
    return (*this->_query)(new_request);
}

Json::Value ACCOUNT_V1::by_riot_id(const std::string& routing, const std::string& gameName, const std::string& tagline) {
    const std::string method_key = "ACCOUNT-V1-by-riot-id";
    const std::array<std::string, 2> method_urls= {"accounts/by-riot-id/", "/"};
    std::shared_ptr<query> new_request = request(routing, method_urls, routing, gameName, tagline);
    return (*this->_query)(new_request);
}

Json::Value ACCOUNT_V1::by_game(const std::string& routing, const std::string& game, const std::string& puuid) {
    const std::string method_key = "ACCOUNT-V1-by-game";
    const std::array<std::string, 2> method_urls= {"active-shards/by-game/", "/by-puuid/"};
    std::shared_ptr<query> new_request = request(routing, method_urls, routing, game, puuid);
    return (*this->_query)(new_request);
}

}
