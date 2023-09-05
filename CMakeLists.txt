cmake_minimum_required(VERSION 3.18.4)
project(RIOTAPICLIENT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GCC_CXX_FLAGS ${GCC_CXX_FLAGS} "-static-libgcc -static-libstdc++ -static -ggdb3 -O3")

find_package(Catch2 3 REQUIRED)

add_subdirectory(src/riot-cpp)


target_compile_options(riot-cpp PRIVATE -fsanitize=address -static-libasan)
target_link_options(riot-cpp PRIVATE -fsanitize=address -static-libasan)


add_executable(client_tests test/client_tests.cpp test/simdjson.cpp)
target_link_libraries(client_tests riot-cpp)
target_link_libraries(client_tests Catch2::Catch2WithMain)

target_compile_options(client_tests PRIVATE -fsanitize=address -static-libasan)
target_link_options(client_tests PRIVATE -fsanitize=address -static-libasan)

add_executable(rate_tests test/Rate_Tests.cpp)
target_link_libraries(rate_tests riot-cpp)
target_link_libraries(rate_tests Catch2::Catch2WithMain)

target_compile_options(rate_tests PRIVATE -fsanitize=address -static-libasan)
target_link_options(rate_tests PRIVATE -fsanitize=address -static-libasan)

