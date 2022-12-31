
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

add_library(client SHARED client.cpp response_codes.cpp 
    queries/LOL/LEAGUE_V4.cpp queries/LOL/SUMMONER_V4.cpp 
    queries/LOL/MATCH_V5.cpp queries/ACCOUNT_V1.cpp 
    queries/LOL/CLASH_V1.cpp)

target_include_directories(client PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(client PRIVATE ${jsoncpp_INCLUDE_DIRS})
target_link_libraries(client ${CURL_LIBRARIES})
target_link_libraries(client ${jsoncpp_LIBRARIES})
