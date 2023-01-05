
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

add_library(client SHARED client.cpp query.cpp response_codes.cpp query_type.cpp)

target_include_directories(client PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(client PRIVATE ${jsoncpp_INCLUDE_DIRS})
target_link_libraries(client ${CURL_LIBRARIES})
target_link_libraries(client ${jsoncpp_LIBRARIES})
