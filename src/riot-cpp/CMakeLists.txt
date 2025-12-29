set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED TRUE)

add_library(riot-cpp STATIC client/client.cpp 
                            handling/rate_hierachy.cpp 
                            handling/handlers.cpp
                            handling/rate_handler.cpp
                            handling/region_count.cpp  
                            logging/logger.cpp
                            types/region_enums.cpp)

target_link_libraries(riot-cpp PRIVATE cpr::cpr)
