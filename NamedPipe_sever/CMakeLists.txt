cmake_minimum_required(VERSION 3.0)

project(pipe_server)

file(GLOB SOURCES "*.c")    

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})