#pragma once

#include <bits/utility.h>
#include <string>
#include <cstring>
#include <string_view>
#include <type_traits>
#include <vector>
#include <functional>
#include <memory>
#include <ctime>
#include <variant>
#include <sstream>

#ifdef DEBUG_MODE
#ifndef rcp_assert
#define rcp_assert(x, msg) if (!x) {std::cerr << "ASSERTION FAILED: " << msg << std::endl;}
#endif
#else
#ifndef rcp_assert
#define rcp_assert(x, msg)
#endif
#endif



namespace riotcpp {
namespace url {
    
    // Emulate kwargs for optional arguments
    template<typename T>
    concept OptInt = std::is_same<T, int>::value ||
                     std::is_same<T, u_int>::value ||
                     std::is_same<T, std::string>::value ||
                     std::is_same<T, std::size_t>::value;
    
    template<OptInt T>
    class Maybe {
        private:
            T value_;
            bool initialised_ = false;
        public:
            Maybe() {};
            Maybe(const T& value) {
                value_ = value;
                initialised_ = true;
            }
            inline T value() const {return value_;};
            inline bool initialised() const {return initialised_;};
    };

    inline u_int early_exit_strlen(const char* buff, u_int max_len) {
        int i = 0;
        while (i < max_len) {
            if (buff[i] == '\0') {
                break;
            }
            ++i;
        }
        return i;
    }

    template<>
    class Maybe<std::string> {
        private: 
            std::string value_;
            bool initialised_ = false;
        public:
            Maybe() {};
            Maybe(const std::string& value) : value_(value) {
                initialised_ = true;
            }
            Maybe(const char* value) {
                // Assert maximum size of 32.
                value_ = std::string(value, early_exit_strlen(value, 32));
                initialised_ = true;
            }
            inline std::string value() const {return value_;};
            inline bool initialised() const {return initialised_;};
    };

    inline bool need_percent_encode(const char chr) {
        if ('@' < chr && chr < '[') {
            return false;
        } 
        if ('`' < chr && chr < '{') {
            return false;
        }
        if ('/' < chr && chr < ':') {
            return false;
        }
        return true;
    }

    inline int length_url_frag(const std::string& param) {
        int n = param.size();
        for (const char& chr : param) {
            n += need_percent_encode(chr) * 2;
        }
        return n;
    }

    inline void to_hex(const char chr, char* encoded) {
        static constexpr char tab[17] = "0123456789ABCDEF";
        encoded[0] = '%';
        encoded[1] = tab[(unsigned char) chr >> 4];
        encoded[2] = tab[(unsigned char) chr & 0b00001111];
        return;
    }

    inline int ndigits(u_int n) {
        int n_digits = 1;
        int comp = 10;
        while (n >= comp) {
            comp *= 10;
            ++n_digits;
        }
        return n_digits;
    }

    inline char* write_int(char* buff, u_int n) {
        int len_n = ndigits(n);
        int tmp;
        for (int i = len_n-1; i >= 0; i--) {
            tmp = n % 10;
            buff[i] = tmp + '0';
            n = (n - tmp) / 10;
        }
        return buff + len_n;
    }
    

    inline char* encode_write(const std::string& to_encode, char* to_write) {
        int ind = 0;
        const char* srt = to_encode.data();
        const std::size_t l = to_encode.size();
        for (int i = 0; i < l; i++) {
            if (need_percent_encode(srt[i])) {
                to_hex(srt[i], to_write + ind);
                ind += 3;
            }
            else {
                to_write[ind] = srt[i];
                ind += 1;
            }
        }
        return to_write + ind;
    }

    typedef struct opt_count {
        Maybe<u_int> count;
    } opt_count;

    typedef struct opt_page {
        Maybe<u_int> page;
    } opt_page;

    typedef struct opt_limit {
        Maybe<u_int> limit;
    } opt_limit;


    typedef struct opt_match_history {
        Maybe<std::size_t> startTime;
        Maybe<std::size_t> endTime;
        Maybe<std::string> queue;
        Maybe<std::string> type;
        Maybe<u_int> start;
        Maybe<u_int> count;
    } opt_match_history;

    typedef struct opt_tft_match_history {
        Maybe<std::size_t> startTime;
        Maybe<std::size_t> endTime;
        Maybe<u_int> start;
        Maybe<u_int> count;
    } opt_tft_match_history;

    typedef struct opt_queue {
        Maybe<std::string> queue;
    } opt_queue;

    typedef struct opt_queue_page {
        Maybe<std::string> queue;
        Maybe<u_int> page;
    } opt_queue_page;

    typedef struct opt_start_size {
        Maybe<u_int> size;
        Maybe<u_int> startIndex;
    } opt_start_size;

    typedef struct opt_locale {
        Maybe<std::string> locale;
    } opt_locale;

    typedef struct no_opt {
    } no_opt;

    template <typename Arg>
    concept OptArg = std::is_same<Arg, opt_count            >::value ||
                     std::is_same<Arg, opt_page             >::value ||
                     std::is_same<Arg, opt_limit            >::value ||
                     std::is_same<Arg, opt_match_history    >::value ||
                     std::is_same<Arg, opt_tft_match_history>::value ||
                     std::is_same<Arg, opt_queue_page       >::value ||
                     std::is_same<Arg, opt_queue            >::value ||
                     std::is_same<Arg, opt_start_size       >::value ||
                     std::is_same<Arg, opt_locale           >::value ||
                     std::is_same<Arg, no_opt               >::value;
    
    template<OptArg T>
    inline void write_opt_arg(char* buff, const T& opt_arg);

    template<>
    inline void write_opt_arg<opt_count>(char* buff, const opt_count& opt_arg) {
        if (opt_arg.count.initialised()) {
            buff[0] = '?'; ++buff;
            memcpy(buff, "count=", 6); buff = buff + 6;
            write_int(buff, opt_arg.count.value());
        }
    }
    template<>
    inline void write_opt_arg<opt_page>(char* buff, const opt_page& opt_arg) {
        if (opt_arg.page.initialised()) {
            buff[0] = '?'; ++buff;
            memcpy(buff, "page=", 5); buff = buff + 5;
            write_int(buff, opt_arg.page.value());
        }
    }
    template<>
    inline void write_opt_arg<opt_limit>(char* buff, const opt_limit& opt_arg) {
        if (opt_arg.limit.initialised()) {
            buff[0] = '?'; ++buff;
            memcpy(buff, "limit=", 6); buff = buff + 6;
            write_int(buff, opt_arg.limit.value());
        }
    }
    template<>
    inline void write_opt_arg<opt_tft_match_history>(char* buff, const opt_tft_match_history& opt_arg) {
        char sep = '?';
        if (opt_arg.startTime.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "startTime=", 10); buff = buff + 10;
            buff = write_int(buff, opt_arg.startTime.value());
            sep = '&';
        }
        if (opt_arg.endTime.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "endTime=", 8); buff = buff + 8;
            buff = write_int(buff, opt_arg.endTime.value());
            sep = '&';
        }
        if (opt_arg.start.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "start=", 6); buff = buff + 6;
            buff = write_int(buff, opt_arg.start.value());
            sep = '&';
        }
        if (opt_arg.count.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "count=", 6); buff = buff + 6;
            buff = write_int(buff, opt_arg.count.value());
            sep = '&';
        }
    }
    template<>
    inline void write_opt_arg<opt_match_history>(char* buff, const opt_match_history& opt_arg) {
        char sep = '?';
        if (opt_arg.startTime.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "startTime=", 10); buff = buff + 10;
            buff = write_int(buff, opt_arg.startTime.value());
            sep = '&';
        }
        if (opt_arg.endTime.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "endTime=", 8); buff = buff + 8;
            buff = write_int(buff, opt_arg.endTime.value());
            sep = '&';
        }
        if (opt_arg.queue.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "queue=", 6); buff = buff + 6;
            buff = encode_write(opt_arg.queue.value(), buff);
            sep = '&';
        }
        if (opt_arg.type.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "type=", 5); buff = buff + 5;
            buff = encode_write(opt_arg.type.value(), buff);
            sep = '&';
        }
        if (opt_arg.start.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "start=", 6); buff = buff + 6;
            buff = write_int(buff, opt_arg.start.value());
            sep = '&';
        }
        if (opt_arg.count.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "count=", 6); buff = buff + 6;
            buff = write_int(buff, opt_arg.count.value());
            sep = '&';
        }
    }
    template<>
    inline void write_opt_arg<opt_queue>(char* buff, const opt_queue& opt_arg) {
        if (opt_arg.queue.initialised()) {
            buff[0] = '?'; ++buff;
            memcpy(buff, "queue=", 6); buff = buff + 6;
            buff = encode_write(opt_arg.queue.value(), buff);
        }
    }
    template<>
    inline void write_opt_arg<opt_queue_page>(char* buff, const opt_queue_page& opt_arg) {
        char sep = '?';
        if (opt_arg.queue.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "queue=", 6); buff = buff + 6;
            buff = encode_write(opt_arg.queue.value(), buff);
            sep = '&';
        }
        if (opt_arg.page.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "page=", 5); buff = buff + 5;
            buff = write_int(buff, opt_arg.page.value());
            sep = '&';
        }
    }
    template<>
    inline void write_opt_arg<opt_start_size>(char* buff, const opt_start_size& opt_arg) {
        char sep = '?';
        if (opt_arg.size.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "size=", 5); buff = buff + 5;
            buff = write_int(buff, opt_arg.size.value());
            sep = '&';
        }
        if (opt_arg.startIndex.initialised()) {
            buff[0] = sep; ++buff;
            memcpy(buff, "startIndex=", 11); buff = buff + 11;
            buff = write_int(buff, opt_arg.startIndex.value());
            sep = '&';
        }
    }
    template<>
    inline void write_opt_arg<opt_locale>(char* buff, const opt_locale& opt_arg) {
        if (opt_arg.locale.initialised()) {
            buff[0] = '?'; ++buff;
            memcpy(buff, "locale=", 7); buff = buff + 7;
            encode_write(opt_arg.locale.value(), buff);
        }
    }
    // Get length of string ?<keywords>=<arg>&<keyword>=<arg>...
    template<OptArg T> 
    inline u_int getSize(const T& opt_arg);
    
    template<>
    inline u_int getSize<opt_count>(const opt_count& opt_arg) {
        if (opt_arg.count.initialised()) {
            return 7 + ndigits(opt_arg.count.value());
        }
        return 0;
    };
    template<>
    inline u_int getSize<opt_page>(const opt_page& opt_arg) {
        if (opt_arg.page.initialised()) {
            return 6 + ndigits(opt_arg.page.value());
        }
        return 0;
    };
    template<>
    inline u_int getSize<opt_limit>(const opt_limit& opt_arg) {
        if (opt_arg.limit.initialised()) {
            return 7 + ndigits(opt_arg.limit.value());
        }
        return 0;
    };
    template<>
    inline u_int getSize<opt_tft_match_history>(const opt_tft_match_history& opt_arg) {
        int size = 0;
        if (opt_arg.startTime.initialised()) {
            size += 11 + ndigits(opt_arg.startTime.value()); 
        }
        if (opt_arg.endTime.initialised()) {
            size += 9 + ndigits(opt_arg.endTime.value()); 
        }
        if (opt_arg.start.initialised()) {
            size += 7 + ndigits(opt_arg.start.value()); 
        }
        if (opt_arg.count.initialised()) {
            size += 7 + ndigits(opt_arg.count.value()); 
        }
        return size;
    };
    template<>
    inline u_int getSize<opt_match_history>(const opt_match_history& opt_arg) {
        int size = 0;
        if (opt_arg.startTime.initialised()) {
            size += 11 + ndigits(opt_arg.startTime.value()); 
        }
        if (opt_arg.endTime.initialised()) {
            size += 9 + ndigits(opt_arg.endTime.value()); 
        }
        if (opt_arg.queue.initialised()) {
            size += 7 + length_url_frag(opt_arg.queue.value()); 
        }
        if (opt_arg.type.initialised()) {
            size += 6 + length_url_frag(opt_arg.type.value()); 
        }
        if (opt_arg.start.initialised()) {
            size += 7 + ndigits(opt_arg.start.value()); 
        }
        if (opt_arg.count.initialised()) {
            size += 7 + ndigits(opt_arg.count.value()); 
        }
        return size;
    };
    template<>
    inline u_int getSize<opt_queue>(const opt_queue& opt_arg) {
        int count = 0;
        if (opt_arg.queue.initialised()) {
            count += 7 + length_url_frag(opt_arg.queue.value());
        }
        return count;
    }
    template<>
    inline u_int getSize<opt_queue_page>(const opt_queue_page& opt_arg) {
        int count = 0;
        if (opt_arg.queue.initialised()) {
            count += 7 + length_url_frag(opt_arg.queue.value());
        }
        if (opt_arg.page.initialised()) {
            count += 6 + ndigits(opt_arg.page.value());
        }
        return count;
    }
    template<>
    inline u_int getSize<opt_start_size>(const opt_start_size& opt_arg) {
        int count = 0;
        if (opt_arg.size.initialised()) {
            count += 6 + ndigits(opt_arg.size.value());
        }
        if (opt_arg.startIndex.initialised()) {
            count += 12 + ndigits(opt_arg.startIndex.value());
        }
        return count;
    }
    template<>
    inline u_int getSize<opt_locale>(const opt_locale& opt_arg) {
        if (opt_arg.locale.initialised()) {
            return 8 + length_url_frag(opt_arg.locale.value());
        }
        return 0;
    }

    template<typename Arg, typename = typename std::enable_if<std::is_arithmetic_v<Arg> || std::is_convertible_v<Arg, std::string_view>>::type>
    inline u_int length_(const Arg& arg) {
        if constexpr (std::is_arithmetic_v<Arg>) {
            return ndigits(arg);
        }
        if constexpr (std::is_convertible_v<Arg, std::string_view>) {
            return length_url_frag(arg);
        }
        rcp_assert(false, "length_ given invalid type");
        return 0;
    }
    
    inline u_int length() {
        return 0;
    }

    template<typename Arg>
    inline u_int length(const Arg& arg) {
        return length_(arg);
    }
    
    template<typename Arg, typename...Args>
    inline u_int length(const Arg& arg, const Args&...args) {
        return length_(arg) + length(args...);
    }

    template<std::size_t N, typename Arg>
    inline void write_url_frag(char* url, const std::size_t ind, const std::array<std::string, N>& frags, const Arg& arg) {
        if constexpr (N == 0) {
            return;
        }
        memcpy(url, frags[ind].data(), frags[ind].size());
        url = url + frags[ind].size();
        if constexpr (std::is_convertible_v<Arg, int>) {
            url = write_int(url, arg);
        }
        else if constexpr (std::is_convertible_v<Arg, std::string_view>) {
            url = encode_write(arg, url);
        }
    }

    template<std::size_t N, typename Arg, typename...Args> 
    inline void write_url_frag(char* url, const std::size_t ind, const std::array<std::string, N>& frags, const Arg& arg, const Args&... args) {
        if constexpr (N == 0) {
            return;
        }
        memcpy(url, frags[ind].data(), frags[ind].size());
        url = url + frags[ind].size();
        if constexpr (std::is_convertible_v<Arg, int>) {
            url = write_int(url, arg);
        }
        else if constexpr (std::is_convertible_v<Arg, std::string_view>) {
            url = encode_write(arg, url);
        }
        if (ind < N-1) {
            write_url_frag(url, ind+1, frags, args...);
        }
    }

    template<std::size_t N, typename...Args>
    std::unique_ptr<char[]> construct_url(const std::string& routing, const std::string& url_base, const std::size_t len, const std::array<std::string, N>& url_frags, const Args&...args) {
        const u_int arg_len = length(args...) + 8 + 18 + routing.size() + url_base.size() + len;
        std::unique_ptr<char[]> url(new char[arg_len+1]);
        url.get()[arg_len] = '\0';
        int ind = 0;
        memcpy(url.get(), "https://", 8);
        ind += 8;
        memcpy(url.get() + ind, routing.data(), routing.size());
        ind += routing.size();
        memcpy(url.get() + ind, ".api.riotgames.com", 18);
        ind += 18;
        memcpy(url.get() + ind, url_base.data(), url_base.size());
        ind += url_base.size();
        if constexpr (N != 0 | sizeof...(Args) == 0) {
            write_url_frag(url.get()+ind, 0, url_frags, args...);
        }
        return url;
    };
    
    template<std::size_t N, typename...Args>
    std::unique_ptr<char[]> construct_url(const std::string& routing, const std::string& url_base, const std::string& trailing, const std::size_t len, const std::array<std::string, N>& url_frags, const Args&...args) {
        const u_int arg_len = length(args...) + 8 + 18 + routing.size() + url_base.size() + len + trailing.size();
        std::unique_ptr<char[]> url(new char[arg_len+1]);
        url.get()[arg_len] = '\0';
        int ind = 0;
        memcpy(url.get(), "https://", 8);
        ind += 8;
        memcpy(url.get() + ind, routing.data(), routing.size());
        ind += routing.size();
        memcpy(url.get() + ind, ".api.riotgames.com", 18);
        ind += 18;
        memcpy(url.get() + ind, url_base.data(), url_base.size());
        ind += url_base.size();
        if constexpr (N != 0 || sizeof...(Args) != 0) {
            write_url_frag(url.get()+ind, 0, url_frags, args...);
        }
        memcpy(url.get() + arg_len - trailing.size(), trailing.data(), trailing.size());
        return url;
    };

    template<OptArg T, std::size_t N, typename...Args>
    std::unique_ptr<char[]> construct_url(const std::string& routing, const std::string url_base, const T& opt_arg, const std::string& trailing, const std::size_t len, const std::array<std::string, N>& url_frags, const Args&...args) {
        const int optsize = getSize(opt_arg);
        const u_int arg_len = length(args...) + 8 + 18 + routing.size() + url_base.size() + len + trailing.size() + optsize;
        std::unique_ptr<char[]> url(new char[arg_len+1]);
        url.get()[arg_len] = '\0';
        int ind = 0;
        memcpy(url.get(), "https://", 8);
        ind += 8;
        memcpy(url.get() + ind, routing.data(), routing.size());
        ind += routing.size();
        memcpy(url.get() + ind, ".api.riotgames.com", 18);
        ind += 18;
        memcpy(url.get() + ind, url_base.data(), url_base.size());
        ind += url_base.size();
        if constexpr (N != 0 || sizeof...(Args) != 0) {
            write_url_frag(url.get()+ind, 0, url_frags, args...);
        }
        memcpy(url.get() + arg_len - trailing.size() - optsize, trailing.data(), trailing.size());
        write_opt_arg(url.get() + arg_len - optsize, opt_arg);
        return url;
    }
    template<OptArg T, std::size_t N, typename...Args>
    std::unique_ptr<char[]> construct_url(const std::string& routing, const std::string url_base, const T& opt_arg, const std::size_t len, const std::array<std::string, N>& url_frags, const Args&...args) {
        return construct_url(routing, url_base, opt_arg, "", len, url_frags, args...);
    }
}
}
