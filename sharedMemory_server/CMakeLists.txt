cmake_minimum_required(VERSION 3.0)

project(sharedMemory_Communication)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")  # 디버그 심볼 정보 포함

set( server server.c )

file(GLOB HEADERS "*.h")
include_directories(${CMAKE_SOURCE_DIR}/include)



add_executable(server ${server})


