#include <chrono>
#include <thread>
#include <ctime>
#include <stdexcept>

namespace rate {

RateLimiter::RateLimiter(CURL* handle, std::shared_ptr<Json::Reader> reader) {
    this->handle = handle;
    this->reader = reader;
}

RateLimiter::~RateLimiter() {};

void RateLimiter::handle_limit() {

    Json::Value header;
    bool parsed = this->reader->parse(this->header_buffer, header);
    if (!parsed) {
        throw std::domain_error("Unable to read response heade");
    }

    int pause_duration = this->extract_duration(header);
    this->pause_for(pause_duration);
    this->header_buffer.clear();
    this->n_attempts += 1;

    return;
}

void RateLimiter::pause_for(int duration) {
    std::this_thread::sleep_for(std::chrono::seconds(duration));
}

int RateLimiter::extract_duration(Json::Value header) {
    try {
        int duration = header["Retry-After"];
        return duration;
    }
    catch (Json::Exception::out_of_range &error) {
        throw Json::Exception::out_of_range("Retry-After not found in response header");
    }
}


}
