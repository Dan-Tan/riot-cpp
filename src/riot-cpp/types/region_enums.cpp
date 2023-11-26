#include "region_enums.h"

#include <array>

namespace riotcpp {
namespace args {
    
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
        return NULL; // not possible 
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
} 
}
