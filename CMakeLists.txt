cmake_minimum_required(VERSION 3.10)
project(metar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(metar
    src/main.cpp
    src/WebHandler.cpp
)

find_package(CURL REQUIRED)
target_link_libraries(metar PRIVATE CURL::libcurl)
