#pragma once

#include <string>
#include <string_view>
#include <type_traits>
#include <memory>
#include <sstream>
#include <array>
#include <vector>

#include <cpr/util.h>

namespace riotcpp::url {
    /**
     * Constructs the base URL path (everything before the query string '?').
     * It interleaves static path fragments with dynamic arguments.
     * @param routing The routing value for the server (e.g., "na1", "americas").
     * @param url_base The base path for the API version (e.g., "/lol/summoner/v4").
     * @param additional_frag A final static fragment to append to the path.
     * @param url_fragments An array of static path parts.
     * @param args The dynamic arguments to be interleaved with the fragments. String-like
     *             arguments will be URL-encoded.
     * @return The complete URL path as a std::string.
     */
    template<std::size_t N, typename... Args>
    std::string construct_url_path(const std::string& routing,
                                   const std::string& url_base,
                                   const std::string& additional_frag,
                                   const std::array<std::string, N>& url_fragments,
                                   const Args&... args) {
        // Ensure that the number of static URL parts matches the number of dynamic arguments.
        static_assert(sizeof...(args) == N, "The number of URL fragments must correspond to the number of arguments.");

        std::stringstream url_ss;
        url_ss << "https://" << routing << ".api.riotgames.com" << url_base;

        // Use a C++17 fold expression to interleave the static fragments and the dynamic arguments.
        if constexpr (sizeof...(args) > 0) {
            std::size_t i = 0;
            auto process_arg = [&](const auto& arg) {
                url_ss << url_fragments[i++];
                // URL-encode arguments that are convertible to a string view.
                if constexpr (std::is_convertible_v<decltype(arg), std::string_view>) {
                    url_ss << cpr::util::urlEncode(arg);
                } else {
                    url_ss << arg;
                }
            };
            (process_arg(args), ...);
        }

        url_ss << additional_frag;
        return url_ss.str();
    }
} // namespace riotcpp::url
