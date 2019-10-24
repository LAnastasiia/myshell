cmake_minimum_required(VERSION 3.15)
project(myshell)

set(CMAKE_CXX_STANDARD 17)
set(ICU_MAJOR_VER "57")
set(ICU_FULL_VERSION "${ICU_MAJOR_VER}.1")

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Boost REQUIRED system)

add_executable(myshell main.cpp internal_functions/functions.cpp)

target_link_libraries( myshell
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${ICU_LIBRARIES})