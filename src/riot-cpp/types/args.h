#pragma once

#include <string>
#include <optional>

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

    template<typename T>
    class ArgType {
        protected:
            T value_;
            bool valid_;

        public:
            ArgType<T>(T value, bool valid) {
                this->value_ = value;
                this->valid_ = valid;
            };
            ArgType<T>(T value) {
                this->value_ = value;
                this->valid_ = true;
            };
            virtual ~ArgType<T>() = 0;
            
            bool is_valid() const {return this->valid;}
    };

    class puuid : public ArgType<std::string> {
        private:  
            std::string puuid_;
            bool valid_;
        public:
            puuid(const std::string& p_uuid) : ArgType<std::string>(p_uuid, p_uuid.length() == 78) {}
    };
}
}
