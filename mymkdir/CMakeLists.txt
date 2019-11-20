cmake_minimum_required(VERSION 3.13)
project(mymkdir)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Boost REQUIRED system)

set(ICU_MAJOR_VER "57")
set(ICU_FULL_VERSION "${ICU_MAJOR_VER}.1")

add_executable(mymkdir main.cpp my_args_parser.cpp my_config.cpp)

target_link_libraries(mymkdir
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${ICU_LIBRARIES})