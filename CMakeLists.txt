cmake_minimum_required(VERSION 3.18.4)
project(RIOTAPICLIENT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
IF (DEBUG_MODE)
    set(GCC_CXX_FLAGS ${GCC_CXX_FLAGS} "-static-libgcc -static-libstdc++ -static -ggdb3 -O0")
ELSE(DEBUG_MODE)
    set(GCC_CXX_FLAGS ${GCC_CXX_FLAGS} "-static-libgcc -static-libstdc++ -static -ggdb3 -O3")
ENDIF(DEBUG_MODE)

find_package(Catch2 3 REQUIRED)

IF (URL_TESTS) 
    add_executable(url_tests test/url_tests.cpp)
    target_link_libraries(url_tests Catch2::Catch2WithMain)
ENDIF(URL_TESTS)

add_subdirectory(src/riot-cpp)

target_compile_options(riot-cpp PRIVATE -fsanitize=address -static-libasan)
target_link_options(riot-cpp PRIVATE -fsanitize=address -static-libasan)

add_executable(client_tests test/client_tests.cpp)
target_link_libraries(client_tests riot-cpp)
target_link_libraries(client_tests Catch2::Catch2)

target_compile_options(client_tests PRIVATE -fsanitize=address -static-libasan)
target_link_options(client_tests PRIVATE -fsanitize=address -static-libasan)


IF(BUILD_TESTING)
    add_executable(rate_tests test/Rate_Tests.cpp)
    target_link_libraries(rate_tests riot-cpp)
    target_link_libraries(rate_tests Catch2::Catch2WithMain)

    target_compile_options(rate_tests PRIVATE -fsanitize=address -static-libasan)
    target_link_options(rate_tests PRIVATE -fsanitize=address -static-libasan)

    add_executable(util_tests test/type_tests.cpp)
    target_link_libraries(util_tests riot-cpp)
    target_link_libraries(util_tests Catch2::Catch2WithMain)

    target_compile_options(util_tests PRIVATE -fsanitize=address -static-libasan)
    target_link_options(util_tests PRIVATE -fsanitize=address -static-libasan)
ENDIF(BUILD_TESTING)
