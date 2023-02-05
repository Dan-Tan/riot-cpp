find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

add_library(riot-cpp SHARED client/client.cpp query/query.cpp query/query_type.cpp handling/structures/request_history.cpp handling/handlers.cpp logging/logger.cpp)

target_include_directories(riot-cpp PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(riot-cpp PRIVATE ${jsoncpp_INCLUDE_DIRS})
target_link_libraries(riot-cpp ${CURL_LIBRARIES})
target_link_libraries(riot-cpp ${jsoncpp_LIBRARIES})
