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

option(RIOTCPP_BUILD_TESTS "Build tests for riot-cpp" OFF)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(ENABLE_ASAN "Enable AddressSanitizer" ON)
    set(RIOTCPP_BUILD_TESTS ON)
else()
    option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

if (RIOTCPP_BUILD_TESTS)
    find_package(Catch2 3 CONFIG QUIET)
    if (NOT Catch2_FOUND)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.5.2
        )
        FetchContent_MakeAvailable(Catch2)
    endif()

    IF (URL_TESTS) 
        add_executable(url_tests test/url_tests.cpp)
        target_link_libraries(url_tests Catch2::Catch2WithMain)
    ENDIF(URL_TESTS)
endif()

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.14.1
)
set(CPR_CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.16.0
)
FetchContent_MakeAvailable(spdlog)

add_subdirectory(src/riot-cpp)

if(ENABLE_ASAN)
    target_compile_options(riot-cpp PRIVATE -fsanitize=address -static-libasan)
    target_link_options(riot-cpp PRIVATE -fsanitize=address -static-libasan)
endif()

if(RIOTCPP_BUILD_TESTS)
    add_executable(client_tests test/client_tests.cpp)
    target_link_libraries(client_tests riot-cpp cpr::cpr Catch2::Catch2)
    if(ENABLE_ASAN)
        target_compile_options(client_tests PRIVATE -fsanitize=address -static-libasan)
        target_link_options(client_tests PRIVATE -fsanitize=address -static-libasan)
    endif()

    IF(BUILD_TESTING)
        add_executable(rate_tests test/Rate_Tests.cpp)
        target_link_libraries(rate_tests riot-cpp Catch2::Catch2WithMain)
        if(ENABLE_ASAN)
            target_compile_options(rate_tests PRIVATE -fsanitize=address -static-libasan)
            target_link_options(rate_tests PRIVATE -fsanitize=address -static-libasan)
        endif()

        add_executable(util_tests test/type_tests.cpp)
        target_link_libraries(util_tests riot-cpp Catch2::Catch2WithMain)
        if(ENABLE_ASAN)
            target_compile_options(util_tests PRIVATE -fsanitize=address -static-libasan)
            target_link_options(util_tests PRIVATE -fsanitize=address -static-libasan)
        endif()
    ENDIF(BUILD_TESTING)
endif()

