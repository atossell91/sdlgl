cmake_minimum_required(VERSION 3.11.2)

project(sdlgl)

set(SRC ${CMAKE_SOURCE_DIR}/src)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(SDL3)

add_executable(sdlgl
        ${SRC}/glad.c
        ${SRC}/main.cpp
)

target_link_libraries(sdlgl SDL3::SDL3)
