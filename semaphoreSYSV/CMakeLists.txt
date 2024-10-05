cmake_minimum_required(VERSION 3.0)

project(semaphore)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(OpenMP REQUIRED)

file(GLOB SOURCES "*.c")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
if(OpenMP_CXX_FOUND)
    target_link_libraries(semaphore PRIVATE OpenMP::OpenMP_CXX)
endif()