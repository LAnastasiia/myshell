cmake_minimum_required(VERSION 3.13)
project(myls)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Boost REQUIRED system)

set(ICU_MAJOR_VER "57")
set(ICU_FULL_VERSION "${ICU_MAJOR_VER}.1")

add_executable(myls main.cpp my_arg_parser.cpp my_config.cpp additionals.cpp comparators.cpp)

target_link_libraries(myls
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${ICU_LIBRARIES})