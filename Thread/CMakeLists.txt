cmake_minimum_required(VERSION 3.0)
project(Thread)
find_package(OpenMP REQUIRED)

file(GLOB SOURCES "*.c")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
if(OpenMP_CXX_FOUND)
    target_link_libraries(Thread PRIVATE OpenMP::OpenMP_CXX)
endif()