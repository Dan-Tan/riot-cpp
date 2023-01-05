#pragma once

#include <string>
#include <cstdarg>
#include <vector>

namespace query {
    class QueryType {
        public:
            QueryType();
            ~QueryType();
            virtual std::string construct_url(std::vector<std::string>) = 0;
            std::string base_url(std::string);
    };

    class U : public QueryType {
        public:
            U(std::string);
            ~U();
            std::string construct_url(std::vector<std::string>) override;

        private:
            std::string u1;
    };

    class UP : public QueryType {
        public:
            UP(std::string);
            ~UP();
            std::string construct_url(std::vector<std::string>) override;

        private:
            std::string u1;
    };

    class UPU : public QueryType {
        public:
            UPU(std::string u1, std::string u2);
            ~UPU();
            std::string construct_url(std::vector<std::string>) override;

        private:
            std::string u1;
            std::string u2;
    };

    class UPP : public QueryType {
        public:
            UPP(std::string);
            ~UPP();
            std::string construct_url(std::vector<std::string>) override;

        private:
            std::string u1;
    };

    class UPUP : public QueryType {
        public:
            UPUP(std::string u1, std::string u2);
            ~UPUP();
            std::string construct_url(std::vector<std::string>) override;

        private:
            std::string u1;
            std::string u2;
    };

    class UPPP : public QueryType {
        public:
            UPPP(std::string);
            ~UPPP();
            std::string construct_url(std::vector<std::string>) override;
        private:
            std::string u1;
    };
}
