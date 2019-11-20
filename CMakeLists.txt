cmake_minimum_required(VERSION 3.13)
project(myshell)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Boost REQUIRED system)
find_package(Boost COMPONENTS program_options REQUIRED)

set(ICU_MAJOR_VER "57")
set(ICU_FULL_VERSION "${ICU_MAJOR_VER}.1")

add_executable(myshell myshell.cpp internal_functions/functions.cpp internal_functions/helper_functions.cpp myShellConfig.cpp)
add_executable(mycat mycat/main.cpp)

target_link_libraries(myshell readline)
target_link_libraries(myshell ${Boost_LIBRARIES})
target_link_libraries( myshell
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${ICU_LIBRARIES})