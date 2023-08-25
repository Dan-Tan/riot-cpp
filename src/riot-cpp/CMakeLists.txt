set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED TRUE)

find_package(CURL REQUIRED)

add_library(riot-cpp STATIC client/client.cpp query/query.cpp handling/structures/request_history.cpp handling/handlers.cpp logging/logger.cpp)

target_include_directories(riot-cpp PRIVATE ${CURL_INCLUDE_DIRS})

target_link_libraries(riot-cpp ${CURL_LIBRARIES})
