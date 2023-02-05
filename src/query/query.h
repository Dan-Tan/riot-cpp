#pragma once
#include <jsoncpp/json/json.h>
#include <ctime>
#include <string>
#include <vector>

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

    using opt_args = std::pair<std::string, std::string>;

    class QueryType {
        public:
            constexpr QueryType() {};
            virtual ~QueryType();

            virtual std::string construct_url(const std::vector<std::string>& params, const std::vector<opt_args>& optional_args = {}) = 0;

        protected:
            inline std::string base_url(std::string region) {
                return "https://" + region + ".api.riotgames.com";
            };

            bool takes_optional_arguments = false;
            std::string query_params(const std::vector<opt_args> optional_args);
    };

    class U : public QueryType {
        public:
            constexpr U(const char *u1, bool optional_args) {
                this->u1 = u1;
                this->takes_optional_arguments = optional_args;
            };
            ~U();
            std::string construct_url(const std::vector<std::string>& params, const std::vector<opt_args>& optional_args = {}) override;

        private:
            const char *u1;
    };

    class UP : public QueryType {
        public:
            constexpr UP(const char *u1, bool optional_args) {
                this->u1 = u1;
                this->takes_optional_arguments = optional_args;
            };
            ~UP();
                std::string construct_url(const std::vector<std::string>& params, const std::vector<opt_args>& optional_args = {}) override;

        private:
            const char *u1;
    };

    class UPU : public QueryType {
        public:
            constexpr UPU(const char *u1, const char *u2, bool optional_args) {
                this->u1 = u1;
                this->u2 = u2;
                this->takes_optional_arguments = optional_args;
            };
            ~UPU();
                std::string construct_url(const std::vector<std::string>& params, const std::vector<opt_args>& optional_args = {}
                        ) override;

        private:
            const char *u1;
            const char *u2;
    };

    class UPP : public QueryType {
        public:
            constexpr UPP(const char *u1, bool optional_args) {
                this->u1 = u1;
                this->takes_optional_arguments = optional_args;
            };
            ~UPP();
                std::string construct_url(const std::vector<std::string>& params, const std::vector<opt_args>& optional_args = {}
                        ) override;

        private:
            const char *u1;
    };

    class UPUP : public QueryType {
        public:
            constexpr UPUP(const char *u1, const char *u2, bool optional_args){
                this->u1 = u1;
                this->u2 = u2;
                this->takes_optional_arguments = optional_args;
            };
            ~UPUP();
                std::string construct_url(const std::vector<std::string>& params, const std::vector<opt_args>& optional_args = {}
                        ) override;

        private:
            const char *u1;
            const char *u2;
    };

    class UPPP : public QueryType {
        public:
            constexpr UPPP(const char *u1, bool optional_args) {
                this->u1 = u1;
                this->takes_optional_arguments = optional_args;
            };
            ~UPPP();
                std::string construct_url(const std::vector<std::string>& params, const std::vector<opt_args>& optional_args = {}) override;
        private:
            const char *u1;
    };
}
