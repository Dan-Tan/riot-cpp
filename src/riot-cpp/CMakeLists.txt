set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED TRUE)

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY
    ${CLANG_TIDY}
    -fix
    -extra-arg=-Wno-unknown-warning-option
    -extra-arg=-Wno-ignored-qualifiers
    -extra-arg=-Wno-unused-parameter
    -extra-arg=-Wno-c++11-extensions
    -quiet
  )
endif()

add_library(riot-cpp STATIC client/client.cpp 
                            handling/rate_hierachy.cpp 
                            handling/handlers.cpp
                            handling/rate_handler.cpp
                            handling/region_count.cpp  
                            logging/logger.cpp
                            types/region_enums.cpp)

target_link_libraries(riot-cpp PRIVATE cpr::cpr)
