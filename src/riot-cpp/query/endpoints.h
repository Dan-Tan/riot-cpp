#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <memory>
#include <ctime>
#include <variant>
#include <sstream>

#include "../types/args.h"

namespace riotcpp {
namespace query {
    
    typedef struct RiotHeader { // default to extremely slow rate limit successful requests will overwrite these
        char date[32];          // users with invalid api keys will only be able to send a request every 2 minutes
        char app_limit[64]          = "1:120";
        char app_limit_count[64]    = "1:120";
        char method_limit[64]       = "1:120";
        char method_limit_count[64] = "1:120";
        char retry_after[4];
    } RiotHeader;

    typedef struct query {
        std::string method_key;
        args::routing route;
        std::string url;
        std::time_t send_time = 0;
        std::unique_ptr<std::vector<char>> response_content;
        RiotHeader response_header;
        int last_response = -2;
        int server_error_count;
    } query;

    using json_text = std::vector<char>;
    using query_fp = std::function<std::unique_ptr<json_text>(std::shared_ptr<query>)>*;

    template<typename T>
    concept param = requires(std::ostream& os, T a) {os<< a;};

    template<param ArgTypes, param OptTypes>
    class Endpoint;

    template<param...ArgTypes, param...OptTypes>
    class Endpoint< std::tuple<ArgTypes...>, std::tuple<OptTypes...> > {
        private:
            const std::string& url_base_;
            const std::array<std::string, sizeof...(ArgTypes)> url_fragments_;
            const std::array<std::string, sizeof...(OptTypes)> opt_keywords_;

            const std::string method_key_;
            const query_fp get_;

        public:
            Endpoint(
                const std::string& method_key,
                const std::string& url_base, 
                std::array<std::string, sizeof...(ArgTypes)>& url_fragments
            ) : method_key_(method_key),
                url_base_(url_base),
                url_fragments_(url_fragments_) {
            };

            std::unique_ptr<json_text> operator()(const std::string& routing, const ArgTypes&... args, const std::pair<std::string, OptTypes>&... opts);
    };

    template<param ArgType>
    inline void construct_url(std::stringstream& url, const std::pair<ArgType, std::string> arg) {
        url << std::get<0>(arg) << std::get<1>(arg);
    }

    template<param ArgType, param...ArgTypes>
    inline void construct_url(std::stringstream& url, const std::pair<ArgType, std::string> arg, const std::pair<ArgTypes, std::string>&... args) {
        url << std::get<0>(arg) << std::get<1>(arg);
        construct_url(url, args...);
    };

    template<param ArgType>
    inline void construct_optional(std::stringstream& url, const std::pair<std::string, ArgType>& opt_arg) {
        url << std::get<0>(opt_arg) << '=' << std::get<1>(opt_arg);
    }

    template<param ArgType, param...ArgTypes>
    inline void construct_optional(std::stringstream& url, const std::pair<std::string, ArgType>& opt_arg, const std::pair<std::string, ArgTypes>&... opt_args) {
        url << std::get<0>(opt_arg) << '=' << std::get<1>(opt_arg) << "&";
        construct_optional(url, opt_args...);

    }

    template<param...ArgTypes, param...OptTypes>
    std::unique_ptr<json_text> Endpoint< std::tuple<ArgTypes...>, std::tuple<OptTypes...> >::operator()(
        const std::string& routing, const ArgTypes&... args, const std::pair<std::string, OptTypes>&... opts
    ) {
        std::stringstream url;
        url << "https://" << routing << ".api.riotgames.com" << this->url_url_;
        construct_url(url, std::make_pair(args, this->url_fragments_)...);
        if constexpr (sizeof...(OptTypes) > 0) {
            url << "?";
            construct_optional(url, opts...);
        }
        std::shared_ptr<query> new_request = std::make_shared<query>(this->method_key_, args::str_to_routing(routing), url.str());
        new_request->response_content = std::make_unique<json_text>();
        return (*this->get_)(new_request);
    };
}
}
