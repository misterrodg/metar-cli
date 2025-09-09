cmake_minimum_required(VERSION 3.10)
project(metar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(metar
    src/main.cpp
    src/ArgParser.cpp
    src/METARParser.cpp
    src/Utils.cpp
    src/WebHandler.cpp
)

find_package(CURL REQUIRED)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(metar ${CURL_LIBRARIES})
endif()
