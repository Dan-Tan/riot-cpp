#pragma once

#include <string>
#include <optional>

namespace routing {
    enum class platform : int {
        BR1 = 0,
        EUN1,
        EUW1,
        JP1,
        KR,
        LA1,
        LA2,
        NA1,
        OC1,
        TR1,
        RU,
        PH2,
        SG2,
        TH2,
        TW2,
        VN2
    };

    enum class regional : int {
        AMERICAS = 0,
        ASIA,
        EUROPE,
        SEA
    };

    std::string platform_to_str(const platform platfrm) noexcept;
    std::optional<platform> str_to_platform(const std::string& platfrm) noexcept;
    bool valid_platform(const std::string& platfrm) noexcept;

    std::string regional_to_str(const regional regionl) noexcept;
    std::optional<regional> str_to_regional(const std::string& regionl) noexcept;
    bool valid_regional(const std::string& regionl) noexcept;
}
