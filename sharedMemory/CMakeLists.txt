cmake_minimum_required(VERSION 3.0)

project(sharedMemory)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")  # 디버그 심볼 정보 포함

file(GLOB SOURCES "*.c*")
file(GLOB HEADERS "*.h")

include_directories(${CMAKE_SOURCE_DIR})
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
install(FILES ${HEADERS} DESTINATION include)