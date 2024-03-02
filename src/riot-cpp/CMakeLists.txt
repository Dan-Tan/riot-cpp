set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED TRUE)

find_package(CURL REQUIRED)

add_library(riot-cpp STATIC client/client.cpp 
                            query/query.cpp 
                            handling/rate_hierachy.cpp 
                            handling/rate_handler.cpp
                            handling/region_count.cpp  
                            logging/logger.cpp
                            types/region_enums.cpp)

target_include_directories(riot-cpp PRIVATE ${CURL_INCLUDE_DIRS})

target_link_libraries(riot-cpp ${CURL_LIBRARIES})
