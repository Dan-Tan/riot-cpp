#include "args.h"

#include <array>
#include <stdexcept>

namespace riotcpp {
namespace args {

    regional platform_to_regional(platform pform) noexcept {
        switch (pform) {
            case platform::NA1:
            case platform::BR1:
            case platform::LA1:
            case platform::LA2:
                return regional::AMERICAS;
            case platform::KR:
            case platform::JP1:
                return regional::ASIA;
            case platform::EUN1:
            case platform::EUW1:
            case platform::TR1:
            case platform::RU:
                return regional::EUROPE;
            case platform::OC1:
            case platform::PH2:
            case platform::SG2:
            case platform::TH2:
            case platform::TW2:
            case platform::VN2:
                return regional::SEA;
        }
        rcp_assert(false, "Non-exhaustive switch. Check if function if upto date with enum class platform");
        return regional::AMERICAS;
    }
    
    /**
     * String equality but case insensitive for first string. For example
     * "string" == "STRING" true
     * "STRING" == "string" false
     * sTrING == STRING true
     */
    static bool str_equals_upper(const std::string& str1, const char* str2) noexcept {
        const int len = str1.length();
        for (int i = 0; i < len; i++) {
            if (str2[i] == '\0') {
                return false;
            }
            if (str1[i] == str2[i]) {
                continue;
            }
            if (str1[i] != str2[i] && !(str1[i] >= 97 && str2[i] <= 122 && str1[i] - 32 == str2[i])) {
                return false;
            }
        }
        return true;
    }

    std::string platform_to_str(platform platfrm) noexcept {
        switch (platfrm) {
            case platform::BR1:
                return "BR1";
            case platform::EUN1:
                return "EUN1";
            case platform::EUW1:
                return "EUW1";
            case platform::JP1:
                return "JP1";
            case platform::KR:
                return "KR";
            case platform::LA1:
                return "LA1";
            case platform::LA2:
                return "LA2";
            case platform::NA1:
                return "NA1";
            case platform::OC1:
                return "OC1";
            case platform::TR1:
                return "TR1";
            case platform::RU:
                return "RU";
            case platform::PH2:
                return "PH2";
            case platform::SG2:
                return "SG2";
            case platform::TH2:
                return "TH2";
            case platform::TW2:
                return "TW2";
            case platform::VN2:
                return "VN2";
        }
        rcp_assert(false, "Non-exhaustive switch. Check if function if upto date with enum class platform");
        return NULL;
    }

    std::optional<platform> str_to_platform(const std::string& platfrm) noexcept {
        constexpr std::array<char[5], 16> platforms {"BR1","EUN1","EUW1","JP1","KR","LA1","LA2","NA1","OC1","TR1","RU","PH2","SG2","TH2","TW2","VN2"};
        int platform_index= 0;
        for (auto& pform : platforms) {
            if (str_equals_upper(platfrm, pform)) {
                return std::optional<platform>{static_cast<platform>(platform_index)};
            }
            platform_index++;
        }
        return std::nullopt;
    }

    /**
     * wrapper of str_to_platform
     * @param string to check if valid platform
     * @return true if valid platform string false otherwise
     */
    bool valid_platform(const std::string& platfrm) noexcept {
        std::optional<platform> pform = str_to_platform(platfrm);
        return pform.has_value();
    }

    std::string regional_to_str(const regional regionl) noexcept {
        switch (regionl) {
            case regional::AMERICAS:
                return "AMERICAS";
            case regional::ASIA:
                return "ASIA";
            case regional::EUROPE:
                return "EUROPE";
            case regional::SEA:
                return "SEA";
        }
        rcp_assert(false, "Non-exhaustive switch. Check if function if upto date with enum class regional");
        return NULL; // not possible 
    }

    std::optional<regional> str_to_regional(const std::string& regionl) noexcept {
        constexpr std::array<char[9], 4> regionals {"AMERICAS", "ASIA", "EUROPE", "SEA"};
        int reg_index = 0;
        for (auto& reg : regionals) {
            if (str_equals_upper(regionl, reg)) {
                return std::optional<regional>{static_cast<regional>(reg_index)};
            }
            reg_index++;
        }
        return std::nullopt;
    }
    
    /**
     * wrapper of str_to_regional
     * @param string to check if valid region
     * @return true if valid region string false otherwise
     */
    bool valid_regional(const std::string& regionl) noexcept {
        std::optional<regional> reg = str_to_regional(regionl);
        return reg.has_value();
    }

    std::string val_platform_to_str(const val_platform vplatform) noexcept {
        switch (vplatform) {
            case val_platform::AP:
                return "AP";
            case val_platform::BR:
                return "BR";
            case val_platform::EU:
                return "EU";
            case val_platform::KR:
                return "KR";
            case val_platform::LATAM:
                return "LATAM";
            case val_platform::NA:
                return "NA";
            case val_platform::ESPORTS:
                return "ESPORTS";
        }
        rcp_assert(false, "Non-exhaustive switch. Check if function if upto date with enum class val_platform");
        return NULL;
    }

    std::optional<val_platform> str_to_val_platform(const std::string& vpform) noexcept {
        constexpr std::array<char[8], 7> val_platforms {"AP", "BR", "EU", "KR", "LATAM", "NA", "ESPORTS"};
        int pform_index = 0;
        for (auto& vform : val_platforms) {
            if (str_equals_upper(vpform, vform)) {
                return std::optional<val_platform>(static_cast<val_platform>(pform_index));
            }
            pform_index++;
        }
        return std::nullopt;
    }

    /**
     * wrapper of str_to_val_platform
     * @param string to check if valid val platform
     * @return true if valid val platform string false otherwise
     */
    bool valid_val_platform(const std::string& vform) noexcept {
        std::optional<val_platform> pform = str_to_val_platform(vform);
        return pform.has_value();
    }

    std::string division_to_str(const division divi) noexcept {
        switch (divi) {
            case division::I:
                return "I";
            case division::II:
                return "II";
            case division::III:
                return "III";
            case division::IV:
                return "IV";
        }
        rcp_assert(false, "Non-exhaustive switch. Check if function if upto date with enum class division");
        return NULL;
    }

    std::optional<division> str_to_division(const std::string& divi) noexcept {
        constexpr std::array<char[4], 4> divisions {"I", "II", "III", "IV"};
        int division_index = 0;
        for (auto& divisin : divisions) {
            if (str_equals_upper(divi, divisin)) {
                return std::optional<division>{static_cast<division>(division_index)};
            }
            division_index++;
        }
        return std::nullopt;

    }

    /**
     * wrapper of str_to_division
     * @param string to check if valid division
     * @return true if valid division string false otherwise
     */
    bool valid_division(const std::string& divi) noexcept {
        std::optional<division> div = str_to_division(divi);
        return div.has_value();
    }

    std::string tier_to_str(const tier tir) noexcept {
        switch (tir) {
            case tier::IRON:
                return "IRON";
            case tier::BRONZE:
                return "BRONZE";
            case tier::SILVER:
                return "SILVER";
            case tier::GOLD:
                return "GOLD";
            case tier::PLATINUM:
                return "PLATINUM";
            case tier::EMERALD:
                return "EMERALD";
            case tier::DIAMOND:
                return "DIAMOND";
        }
        rcp_assert(false, "Non-exhaustive switch. Check if function if upto date with enum class tier");
        return NULL;
    }

    std::optional<tier> str_to_tier(const std::string& tir) noexcept {
        constexpr std::array<char[9], 7> tiers {"IRON", "BRONZE", "SILVER", "GOLD", "PLATINUM", "EMERALD", "DIAMOND"};
        int tier_index = 0;
        for (auto& tirs : tiers) {
            if (str_equals_upper(tir, tirs)) {
                return std::optional<tier>{static_cast<tier>(tier_index)};
            }
            tier_index++;
        }
        return std::nullopt;
    }

    /**
     * wrapper of str_to_tier
     * @param string to check if valid tier
     * @return true if valid tier string false otherwise
     */
    bool valid_tier(const std::string& tir) noexcept {
        std::optional<division> ti = str_to_division(tir);
        return ti.has_value();
    }

    std::string queue_to_str(const queue que) noexcept {
        switch (que) {
            case queue::RANKED_SOLO_5x5:
                return "RANKED_SOLO_5x5";
            case queue::RANKED_FLEX_SR:
                return "RANKED_FLEX_SR";
            case queue::RANKED_FLEX_TT:
                return "RANKED_FLEX_TT";
        }
        rcp_assert(false, "Non-exhaustive switch. Check if function if upto date with enum class queue");
        return NULL;
    }

    std::optional<queue> str_to_queue(const std::string& que) noexcept {
        constexpr std::array<char[16], 3> queues {"RANKED_SOLO_5x5", "RANKED_FLEX_SR", "RANKED_FLEX_TT"};
        int queue_index = 0;
        for (auto& ques : queues) {
            if (str_equals_upper(que, ques)) {
                return std::optional<queue>{static_cast<queue>(queue_index)};
            }
            queue_index++;
        }
        return std::nullopt;
    }

    /**
     * wrapper of str_to_tier
     * @param string to check if valid tier
     * @return true if valid tier string false otherwise
     */
    bool valid_queue(const std::string& queu) noexcept {
        std::optional<queue> que = str_to_queue(queu);
        return que.has_value();
    }

    /**
     * Converts a string to a routing struct. 
     * Importantly this function throws an invalid arguement error if the arguement is invalid. 
     * Users should never be able to send a request to an invalid routing value as this could compromise
     * their api key.
     * @param string to convert to toute.
     * @returns routing
     */
    routing str_to_routing(const std::string& route) {
        std::optional<platform> pform = str_to_platform(route);
        if (pform.has_value()) {
            return routing(pform.value());
        }
        std::optional<regional> region = str_to_regional(route);
        if (region.has_value()) {
            return routing(region.value());
        }
        std::optional<val_platform> vform = str_to_val_platform(route);
        if (vform.has_value()) {
            return routing(vform.value());
        }
        // Sending get requests to an unexpected routing address is dangerous.
        throw std::invalid_argument("Invalid routing value given: " + route);
    }
} 
}
