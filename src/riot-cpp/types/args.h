#pragma once

#include <string>
#include <string_view>
#include <optional>

#ifdef DEBUG_MODE
#define rcp_assert(x, msg) if (!x) {std::cerr << "ASSERTION FAILED: " << msg << std::endl;}
#else 
#define rcp_assert(x, msg)
#endif

#define NUM_CHAMPIONS 166

#define NUM_PLATFORMS     16
#define NUM_VAL_PLATFORMS 5
#define NUM_REGIONS       4

#define REGIONAL_INDICATOR     0
#define PLATFORM_INDICATOR     1
#define VAL_PLATFORM_INDICATOR 2

/*
 * Header file for type definitions used as input arguments. 
 * Helps with input sanitisation, allows the client to abort a request before sending if the arguments are known to be invalid
 *
 * [SECTION] Platform and Routing
 */

namespace riotcpp {
namespace args {

    /*
    * [SECTION] Platform and Routing
    */

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

    enum class val_platform : int {
        AP = 0, 
        BR,
        EU,
        KR,
        LATAM,
        NA,
        ESPORTS // 6
    };

    enum class regional : int {
        AMERICAS = 0,
        ASIA,
        EUROPE,
        SEA
    };

    typedef union routing_union {
        regional reg;
        platform pltform;
        val_platform vpltform;
    }routing_union;

    typedef struct routing {
        routing_union routng;
        int indicator;
        constexpr routing(regional     r) { routng.reg      = r; indicator = 0;}
        constexpr routing(platform     p) { routng.pltform  = p; indicator = 1;}
        constexpr routing(val_platform v) { routng.vpltform = v; indicator = 2;}
    } routing;

    routing str_to_routing(const std::string&);

    regional platform_to_regional(platform) noexcept;

    std::string platform_to_str(const platform) noexcept;
    std::optional<platform> str_to_platform(const std::string& platfrm) noexcept;
    bool valid_platform(const std::string&) noexcept;

    std::string regional_to_str(const regional) noexcept;
    std::optional<regional> str_to_regional(const std::string&) noexcept;
    bool valid_regional(const std::string&) noexcept;

    std::string val_platform_to_str(const val_platform) noexcept;
    std::optional<val_platform> str_to_val_platform(const std::string&) noexcept;
    bool valid_val_platform(const std::string&) noexcept;

    enum class division : int {
        I = 0,
        II,
        III,
        IV
    };

    std::string division_to_str(const division) noexcept;
    std::optional<division> str_to_division(const std::string&) noexcept;
    bool valid_division(const std::string&) noexcept;

    enum class tier : int {
        IRON = 0,
        BRONZE,
        SILVER,
        GOLD,
        PLATINUM,
        EMERALD,
        DIAMOND
    };

    std::string tier_to_str(const tier) noexcept;
    std::optional<tier> str_to_tier(const std::string&) noexcept;
    bool valid_tier(const std::string&) noexcept;

    enum class queue : int {
        RANKED_SOLO_5x5 = 0,
        RANKED_FLEX_SR,
        RANKED_FLEX_TT
    };

    std::string queue_to_str(const queue) noexcept;
    std::optional<queue> str_to_queue(const std::string&) noexcept;
    bool valid_queue(const std::string&) noexcept;
}
}
