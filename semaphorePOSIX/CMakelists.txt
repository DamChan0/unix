cmake_minimum_required(VERSION 3.0)

project(semaphore)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "*.c")

add_executable(semaphore ${SOURCES})
