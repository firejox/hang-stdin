cmake_minimum_required(VERSION 3.22)

project(hang-stdin C)

set(CMAKE_C_STANDARD 11)

option(USE_EVENT_FD "Use event fd for blocking" OFF)
option(USE_UNIX_SOCKET "Use unix socket for blocking" OFF)

add_executable(${PROJECT_NAME} main.c)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<AND:$<PLATFORM_ID:Linux>,$<BOOL:${USE_EVENT_FD}>>:USE_EVENT_FD>"
    "$<$<BOOL:${USE_UNIX_SOCKET}>:USE_UNIX_SOCKET>")

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

set(CPACK_PACKAGE_NAME hang-stdin)
set(CPACK_PACKAGE_CONTACT "Firejox firejox@gmail.com")
set(CPACK_GENERATOR DEB)
include(CPack)
