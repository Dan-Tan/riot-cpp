#include <vector>
#include <jsoncpp/json/json.h>
#include <memory>

namespace rate {
    class RateLimiter {

        private:
            CURL* handle;
            std::vector<char> header_buffer;
            int n_attempts = 0;
            std::shared_ptr<Json::Reader> reader;

        public:
            RateLimiter(CURL* handle, std::shared_ptr<Json::Reader>);
            ~RateLimiter();

            void handle_limit();

        private:
            void pause_for(int duration);

            int extract_duration(Json::Value header);
    }
}
