cmake_minimum_required(VERSION 3.18.4)
project(RIOTAPICLIENT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GCC_CXX_FLAGS ${GCC_CXX_FLAGS} "-static-libgcc -static-libstdc++ -static -ggdb3 -O3")

find_package(Catch2 3 REQUIRED)

add_subdirectory(src)

add_executable(client_tests test/client_tests.cpp)
add_executable(rate_tests test/Rate_Tests.cpp)

target_link_libraries(client_tests riot-cpp)
target_link_libraries(client_tests Catch2::Catch2WithMain)

target_link_libraries(rate_tests riot-cpp)
target_link_libraries(rate_tests Catch2::Catch2WithMain)

