#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "../src/riot-cpp/query/url.h"

namespace riotcpp {
namespace url {

    TEST_CASE("Percent Encode Check") {
        // Techincally all character can be percent encoded 
        // however we should at least leave Alpha-numerics
        // plain.

        SECTION("Alpha-numeric") {
            REQUIRE_FALSE(need_percent_encode('a'));
            REQUIRE_FALSE(need_percent_encode('A'));
            REQUIRE_FALSE(need_percent_encode('z'));
            REQUIRE_FALSE(need_percent_encode('Z'));
            REQUIRE_FALSE(need_percent_encode('0'));
            REQUIRE_FALSE(need_percent_encode('9'));
            REQUIRE_FALSE(need_percent_encode('k'));
            REQUIRE_FALSE(need_percent_encode('l'));
            REQUIRE_FALSE(need_percent_encode('s'));

            REQUIRE_FALSE(need_percent_encode('1'));
            REQUIRE_FALSE(need_percent_encode('2'));
            REQUIRE_FALSE(need_percent_encode('3'));
            REQUIRE_FALSE(need_percent_encode('4'));
            REQUIRE_FALSE(need_percent_encode('5'));
            REQUIRE_FALSE(need_percent_encode('6'));
            REQUIRE_FALSE(need_percent_encode('7'));
            REQUIRE_FALSE(need_percent_encode('8'));
        }

        SECTION("RESERVED") {
            //  * ' ( ) ; : @ & = + $ , / ? % # [ ]
            REQUIRE(need_percent_encode('*'));
            REQUIRE(need_percent_encode('\''));
            REQUIRE(need_percent_encode('('));
            REQUIRE(need_percent_encode(')'));
            REQUIRE(need_percent_encode(';'));
            REQUIRE(need_percent_encode(':'));
            REQUIRE(need_percent_encode('@'));
            REQUIRE(need_percent_encode('&'));
            REQUIRE(need_percent_encode('='));
            REQUIRE(need_percent_encode('+'));
            REQUIRE(need_percent_encode('$'));
            REQUIRE(need_percent_encode(','));
            REQUIRE(need_percent_encode('/'));
            REQUIRE(need_percent_encode('?'));
            REQUIRE(need_percent_encode('%'));
            REQUIRE(need_percent_encode('#'));
            REQUIRE(need_percent_encode('['));
            REQUIRE(need_percent_encode(']'));
            REQUIRE(need_percent_encode(' '));
        }

        SECTION("UTF-8 Unit") {
            REQUIRE(need_percent_encode((char) 0x8A));
            REQUIRE(need_percent_encode((char) 0xAA));
            REQUIRE(need_percent_encode((char) 0x9A));
            REQUIRE(need_percent_encode((char) 0xB3));
            REQUIRE(need_percent_encode((char) 0xD8));
            REQUIRE(need_percent_encode((char) 0xBE));
            REQUIRE(need_percent_encode((char) 0x91));
            REQUIRE(need_percent_encode((char) 0xC4));
            REQUIRE(need_percent_encode((char) 0xA8));
            REQUIRE(need_percent_encode((char) 0x8A));
            REQUIRE(need_percent_encode((char) 0xD9));
            REQUIRE(need_percent_encode((char) 0x93));
        }
    }

    TEST_CASE("Length of URL Fragment") {

        SECTION("NO RESERVED") {
            REQUIRE(length_url_frag("1234")         == 4);
            REQUIRE(length_url_frag("123456789")    == 9);
            REQUIRE(length_url_frag("HelloWorld")   == 10);
            REQUIRE(length_url_frag("Testing")      == 7);
            REQUIRE(length_url_frag("namesd3456")   == 10);
        }

        SECTION("CONTAINS RESERVED") {
            REQUIRE(length_url_frag("Hello World") == 13);
            REQUIRE(length_url_frag("He%lo Faker") == 15);
            REQUIRE(length_url_frag("Hello )[]ld") == 19);
            REQUIRE(length_url_frag("%") == 3);
            REQUIRE(length_url_frag("[()]") == 12);
            REQUIRE(length_url_frag("//%%a%%//") == 25);
        }

        SECTION("UTF-8") {
            // Hello World!
            REQUIRE(length_url_frag("你好世界") == 36);
            REQUIRE(length_url_frag("안녕하세요 세계") == 66);
            REQUIRE(length_url_frag("£¥§©") == 24);

            REQUIRE(length_url_frag("£¥§©안녕하세요 세계") == 66 + 24);
            REQUIRE(length_url_frag("£¥§©你好世界") == 24 + 36);
            REQUIRE(length_url_frag("£¥§©안녕하세요 세계你好世界") == 24 + 66 + 36);
        }
    }

    static inline bool percent_encode_equal(const char expected[3], const char received[3]) {
        return expected[0] == received[0] 
            && expected[1] == received[1] 
            && expected[2] == received[2];
    }

    TEST_CASE("Char to Percent Hex") {
        char temp_buff[4]; temp_buff[3] = '\0';
        char expected[4]; expected[3] = '\0'; expected[0] = '%';
        REQUIRE(percent_encode_equal);
        expected[1] = 'A'; expected[2] = '4';
        to_hex((char) 0xA4, temp_buff);
        REQUIRE(percent_encode_equal(expected, temp_buff));
        expected[1] = 'D'; expected[2] = '1';
        to_hex((char) 0xD1, temp_buff);
        REQUIRE(percent_encode_equal(expected, temp_buff));
        expected[1] = '8'; expected[2] = '4';
        to_hex((char) 0x84, temp_buff);
        REQUIRE(percent_encode_equal(expected, temp_buff));
        expected[1] = '1'; expected[2] = 'B';
        to_hex((char) 0x1B, temp_buff);
        REQUIRE(percent_encode_equal(expected, temp_buff));
        expected[1] = '5'; expected[2] = 'E';
        to_hex((char) 0x5E, temp_buff);
        REQUIRE(percent_encode_equal(expected, temp_buff));
        expected[1] = '9'; expected[2] = 'F';
        to_hex((char) 0x9F, temp_buff);
        REQUIRE(percent_encode_equal(expected, temp_buff));
    }

    bool equal_char_buff(const u_int buff_size, const char* expected, const char* received) {
        bool eq = true;
        for (int i = 0; i < buff_size; i++) {
            eq &= expected[i] == received[i];
        }
        return eq;
    }

    TEST_CASE("Encode Write") {
        // Testing for encode_write
        
        SECTION("NO ENCODE") {
            std::vector<std::string> words = {"hello", "world", "no", "encode"};
            const std::vector<u_int> write_len = {5, 10, 12, 18};
            char buff[18];
            const char expected[19] = "helloworldnoencode";

            char* next_write = encode_write(words[0], buff);
            REQUIRE(equal_char_buff(write_len[0], expected, buff));
            next_write = encode_write(words[1], next_write);
            REQUIRE(equal_char_buff(write_len[1], expected, buff));
            next_write = encode_write(words[2], next_write);
            REQUIRE(equal_char_buff(write_len[2], expected, buff));
            next_write = encode_write(words[3], next_write);
            REQUIRE(equal_char_buff(write_len[3], expected, buff));
        }

        SECTION("WITH ENCODED") {
            std::vector<std::string> words = {"hello ", "world ", "no ", "encode"};
            const std::vector<u_int> write_len = {8, 16, 21, 27};
            char buff[27];
            const char expected[28] = "hello%20world%20no%20encode";
            
            char* next_write = encode_write(words[0], buff);
            REQUIRE(equal_char_buff(write_len[0], expected, buff));
            next_write = encode_write(words[1], next_write);
            REQUIRE(equal_char_buff(write_len[1], expected, buff));
            next_write = encode_write(words[2], next_write);
            REQUIRE(equal_char_buff(write_len[2], expected, buff));
            next_write = encode_write(words[3], next_write);
            REQUIRE(equal_char_buff(write_len[3], expected, buff));
        }
    }

    TEST_CASE("Number of Digits") {
        // Testing for ndigits

        REQUIRE(ndigits(12345)    == 5);
        REQUIRE(ndigits(9375)     == 4);
        REQUIRE(ndigits(1)        == 1);
        REQUIRE(ndigits(98284765) == 8);
        REQUIRE(ndigits(2301)     == 4);
        REQUIRE(ndigits(8762345)  == 7);
    }

    TEST_CASE("Optional Arguments Length Tests") {
        // Test getSize for optional argument structs
        
        SECTION("Optional Count") {
            // Expected string &count=1 or ?count=1
            opt_count opt_arg = {.count = 1};
            REQUIRE(getSize(opt_arg) == 8);
            opt_arg = {.count = 13};
            REQUIRE(getSize(opt_arg) == 9);
            opt_arg = {.count = 1234};
            REQUIRE(getSize(opt_arg) == 11);
            opt_arg = {.count = 12345};
            REQUIRE(getSize(opt_arg) == 12);
            opt_arg = {};
            REQUIRE(getSize(opt_arg) == 0);
        }
        SECTION("Optional Page") {
            // Expected string &page=1
            opt_page opt_arg = {.page = 1};
            REQUIRE(getSize(opt_arg) == 7);
            opt_arg = {.page = 13};
            REQUIRE(getSize(opt_arg) == 8);
            opt_arg = {.page = 1234};
            REQUIRE(getSize(opt_arg) == 10);
            opt_arg = {.page = 9876543};
            REQUIRE(getSize(opt_arg) == 13);
            opt_arg = {.page = 132};
            REQUIRE(getSize(opt_arg) == 9);
            opt_arg = {};
            REQUIRE(getSize(opt_arg) == 0);
        }
        SECTION("Optional Limit") {
            // Expected string &limit=1
            opt_limit opt_arg = {.limit = 2};
            REQUIRE(getSize(opt_arg) == 8);
            opt_arg = {.limit = 23};
            REQUIRE(getSize(opt_arg) == 9);
            opt_arg = {.limit = 234};
            REQUIRE(getSize(opt_arg) == 10);
            opt_arg = {.limit = 12345};
            REQUIRE(getSize(opt_arg) == 12);
            opt_arg = {};
            REQUIRE(getSize(opt_arg) == 0);
        }
        SECTION("Optional Match History") {
            // Expected string ?startTime=1&endTime=1&queue="string"&type="string"&start=1&count=1
            opt_match_history opt_arg = {};
            REQUIRE(getSize(opt_arg) == 0);
            opt_arg = {.startTime = 1};
            REQUIRE(getSize(opt_arg) == 12);
            opt_arg = {.endTime = 1};
            REQUIRE(getSize(opt_arg) == 10);
            opt_arg = {.queue = "Ranked_Solo_5x5"};
            REQUIRE(getSize(opt_arg) == 26);
            opt_arg = {.type = "type"};
            REQUIRE(getSize(opt_arg) == 10);
            opt_arg = {.start = 1};
            REQUIRE(getSize(opt_arg) == 8);
            opt_arg = {.count = 12};
            REQUIRE(getSize(opt_arg) == 9);
            opt_arg = {.startTime = 1, .endTime = 12};
            REQUIRE(getSize(opt_arg) == 23);
            opt_arg = {.endTime = 23, .queue = "Ranked_Solo_5x5", .count = 12};
            REQUIRE(getSize(opt_arg) == 46);
            opt_arg = {.endTime = 23, .queue = "Ranked_Solo_5x5", .start = 1, .count = 12};
            REQUIRE(getSize(opt_arg) == 54);
        }
        SECTION("Optional Queue Page") {
            // Expected string ?queue="string"&page=1
            opt_queue_page opt_arg = {};
            REQUIRE(getSize(opt_arg) == 0);
            opt_arg = {.queue = "Ranked_Solo_5x5"};
            REQUIRE(getSize(opt_arg) == 26);
            opt_arg = {.page = 1};
            REQUIRE(getSize(opt_arg) == 7);
            opt_arg = { .queue = "Ranked_Solo_5x5", .page = 1};
            REQUIRE(getSize(opt_arg) == 33);
        }
        SECTION("Optional Start Size") {
            // Expected string ?size=1&startIndex=1
            opt_start_size opt_arg = {};
            REQUIRE(getSize(opt_arg) == 0);
            opt_arg = {.size = 1};
            REQUIRE(getSize(opt_arg) == 7);
            opt_arg = {.startIndex = 1};
            REQUIRE(getSize(opt_arg) == 13);
            opt_arg = {.size = 12, .startIndex = 13};
            REQUIRE(getSize(opt_arg) == 22);
        }
        SECTION("Optional Locale") {
            // Expected string ?locale="string"
            opt_locale opt_arg = {};
            REQUIRE(getSize(opt_arg) == 0);
            opt_arg = {.locale = "locale"};
            REQUIRE(getSize(opt_arg) == 14);
            opt_arg = {.locale = "loc ale"};
            REQUIRE(getSize(opt_arg) == 17);
        }
    }

    TEST_CASE("Variadic Url Fragments") {
        // Testing length function
        REQUIRE(length("par1", "par2", "par3") == 12);
        REQUIRE(length("par1", "par3", 1, 12)  == 11);
        REQUIRE(length("%encode", 1234, "*encode") == 22);
        REQUIRE(length(1, 12, 123, 1234) == 10);
    }

    TEST_CASE("Copy int to buffer") {
        // equal_char_buff
        char expected[] = "123456789";
        char buffer[10];
        char* next_write = write_int(buffer, 1);
        REQUIRE(equal_char_buff(1, expected, buffer));
        next_write = write_int(next_write, 23);
        REQUIRE(equal_char_buff(1, expected, buffer));
        next_write = write_int(next_write, 456);
        REQUIRE(equal_char_buff(1, expected, buffer));
        next_write = write_int(next_write, 789);
        REQUIRE(equal_char_buff(1, expected, buffer));
    }
    
    TEST_CASE("URL Construction") {
        std::string routing = "americas";
        std::string base_url = "/baseurlhere";
        std::array<std::string, 3> url_frags = {"/frag1/", "/frag2/", "/frag3/"};
        std::string expected_url = "https://americas.api.riotgames.com/baseurlhere/frag1/par%201/frag2/pa%20r2/frag3/p%20ar3";
        std::unique_ptr<char[]> url = construct_url(routing, base_url, 21, url_frags, "par 1", "pa r2", "p ar3");
        std::string received = std::string(url.get());
        REQUIRE(received == expected_url);
        expected_url += "/trailing";
        url = construct_url(routing, base_url, "/trailing", 21, url_frags, "par 1", "pa r2", "p ar3");
        received = std::string(url.get());
        REQUIRE(received == expected_url);
        expected_url = std::string("https://americas.api.riotgames.com/baseurlhere/frag1/12/frag2/123/frag3/1234/trailing");
        url = construct_url(routing, base_url, "/trailing", 21, url_frags, 12, 123, 1234);
        received = std::string(url.get());
        REQUIRE(received == expected_url);
        opt_match_history opt_arg = {.startTime = 1, .endTime = 12, .queue = "Ranked_Solo_5x5", .type = "type", .count = 10};
        expected_url = std::string("https://americas.api.riotgames.com/baseurlhere/frag1/12/frag2/123/frag3/1234/trailing?startTime=1&endTime=12&queue=Ranked%5FSolo%5F5x5&type=type&count=10");
        url = construct_url(routing, base_url, opt_arg, "/trailing", 21, url_frags, 12, 123, 1234);
        received = std::string(url.get());
        REQUIRE(received == expected_url);
        opt_arg = {.startTime = 1, .endTime = 12, .queue = "Ranked_Solo_5x5", .type = "type", .count = 10};
        expected_url = std::string("https://americas.api.riotgames.com/baseurlhere/frag1/12/frag2/123/frag3/1234?startTime=1&endTime=12&queue=Ranked%5FSolo%5F5x5&type=type&count=10");
        url = construct_url(routing, base_url, opt_arg, 21, url_frags, 12, 123, 1234);
        received = std::string(url.get());
        REQUIRE(received == expected_url);
    };

}
}
