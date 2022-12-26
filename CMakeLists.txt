cmake_minimum_required(VERSION 3.22.1)
find_package(Catch2 3 REQUIRED)

project(RIOTAPICLIENT)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)

add_executable(tests test.cpp)
add_executable(custom-main-tests test.cpp test-main.cpp)

