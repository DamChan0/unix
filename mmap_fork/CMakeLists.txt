cmake_minimum_required(VERSION 3.0)
project(memoryMapping)

file(GLOB SOURCES "*.c*")

add_executable(memoryMapping ${SOURCES})
