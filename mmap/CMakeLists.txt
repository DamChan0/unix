cmake_minimum_required(VERSION 3.0)
project(memoryMapping)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")  # 디버그 심볼 정보 포함

file(GLOB SOURCES "*.c*")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
