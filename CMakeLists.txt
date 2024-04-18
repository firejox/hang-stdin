cmake_minimum_required(VERSION 3.22)

project(hang-stdin C)

set(CMAKE_C_STANDARD 11)

set(BlockImpl "PIPE" CACHE STRING "BlockImpl can be chosen for different implementation.")
set(BlockImplValues "PIPE;UNIX_SOCKET;EVENT_FD" CACHE INTERNAL
    "List of possible blocking implementation")
set_property(CACHE BlockImpl PROPERTY STRINGS ${BlockImplValues})

message(STATUS "BlockImpl=${BlockImpl}")

add_executable(${PROJECT_NAME} main.c)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<AND:$<PLATFORM_ID:Linux>,$<STREQUAL:$<UPPER_CASE:${BlockImpl}>,EVENT_FD>>:USE_EVENT_FD>"
    "$<$<STREQUAL:$<UPPER_CASE:${BlockImpl}>,UNIX_SOCKET>:USE_UNIX_SOCKET>")

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT "Firejox firejox@gmail.com")
set(CPACK_PACKAGE_VENDOR "Firejox")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "run a command to read stdin blocking forever")
set(CPACK_RESOURCE_FILE_README "README.md")
set(CPACK_RESOURCE_FILE_LICENSE "LICENSE.md")

set(CPACK_GENERATOR DEB)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
include(CPack)
