#project information
cmake_minimum_required(VERSION 2.8)
project(replaycap)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58 COMPONENTS program_options REQUIRED )

#flags
set(CMAKE_CXX_FLAGS "-std=c++11 -msse4.2 ${CMAKE_CXX_FLAGS} -g -O3")

#add include directories
include_directories(
    src
    ${Boost_INCLUDE_DIR})

#rebuild
file(GLOB_RECURSE replaycap_source
    "src/main.cpp"
    "src/cap/*.cpp"
    "src/core/*.cpp"
    "src/core/replay/*.cpp"
    "src/output/if/*.cpp"
    "src/output/tcp/*.cpp"
    "src/output/udp/*.cpp"
    "src/util/*.cpp"
    "src/util/fuzz.cpp")

#exe and linker info
add_executable (replaycap ${replaycap_source})
target_link_libraries(replaycap "-lpcap" "-lpthread" ${Boost_LIBRARIES})

#install path and whatnot
install(TARGETS replaycap DESTINATION bin)
