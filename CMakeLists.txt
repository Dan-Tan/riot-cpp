cmake_minimum_required(VERSION 3.22.1)

find_package(Catch2 3 REQUIRED)
find_package(CURL REQUIRED)

project(RIOTAPICLIENT)

add_executable(tests test/client_tests.cpp)
add_subdirectory(src)
target_link_libraries(tests PRIVATE client)

target_link_libraries(tests PRIVATE curl)
target_link_libraries(tests PRIVATE jsoncpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)


