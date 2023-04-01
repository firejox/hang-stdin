cmake_minimum_required(VERSION 3.13)

project(hang-stdin C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME} main.c)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
