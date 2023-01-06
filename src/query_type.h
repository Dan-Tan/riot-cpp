#pragma once

#include <string>
#include <cstdarg>
#include <vector>

namespace query {
    class QueryType {
        public:
            constexpr QueryType() {};
            ~QueryType();
            virtual std::string construct_url(std::vector<std::string>) = 0;
            inline std::string base_url(std::string region) {
                return "https://" + region + ".api.riotgames.com";
            };
    };

    class U : public QueryType {
        public:
            constexpr U(const char *u1) {
                this->u1 = u1;
            };
            ~U();
            std::string construct_url(std::vector<std::string>) override;

        private:
            const char *u1;
    };

    class UP : public QueryType {
        public:
            constexpr UP(const char *u1) {
                this->u1 = u1;
            };
            ~UP();
            std::string construct_url(std::vector<std::string>) override;

        private:
            const char *u1;
    };

    class UPU : public QueryType {
        public:
            constexpr UPU(const char *u1, const char *u2) {
                this->u1 = u1;
                this->u2 = u2;
            };
            ~UPU();
            std::string construct_url(std::vector<std::string>) override;

        private:
            const char *u1;
            const char *u2;
    };

    class UPP : public QueryType {
        public:
            constexpr UPP(const char *u1) {
                this->u1 = u1;
            };
            ~UPP();
            std::string construct_url(std::vector<std::string>) override;

        private:
            const char *u1;
    };

    class UPUP : public QueryType {
        public:
            constexpr UPUP(const char *u1, const char *u2){
                this->u1 = u1;
                this->u2 = u2;
            };
            ~UPUP();
            std::string construct_url(std::vector<std::string>) override;

        private:
            const char *u1;
            const char *u2;
    };

    class UPPP : public QueryType {
        public:
            constexpr UPPP(const char *u1) {
                this->u1 = u1;
            };
            ~UPPP();
            std::string construct_url(std::vector<std::string>) override;
        private:
            const char *u1;
    };
}
