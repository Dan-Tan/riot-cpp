set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED TRUE)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()
if (WIN32)
	list(APPEND jsoncpp_LIBRARIES "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/jsoncpp.lib")
endif()
if (MACOSX)
	list(APPEND jsoncpp_LIBRARIES "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/jsoncpp.lib")
endif()

find_package(CURL REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_library(riot-cpp STATIC client/client.cpp query/query.cpp handling/structures/request_history.cpp handling/handlers.cpp logging/logger.cpp)

target_include_directories(riot-cpp PRIVATE ${jsoncpp_INCLUDE_DIRS})
target_link_libraries(riot-cpp ${CURL_LIBRARIES})
target_link_libraries(riot-cpp ${jsoncpp_LIBRARIES}) 
