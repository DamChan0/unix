cmake_minimum_required(VERSION 3.0)
project(signal_alarm)

file(GLOB SOURCES "*.c")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
