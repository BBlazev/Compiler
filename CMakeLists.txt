cmake_minimum_required(VERSION 3.25)
project(mycc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC COPY_ON_ERROR)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

file(GLOB_RECURSE MYCC_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(NOT MYCC_SOURCES)
  message(STATUS "No .cpp files in src/ yet. Add one and build again.")
  return()
endif()

add_executable(mycc ${MYCC_SOURCES})
target_include_directories(mycc PRIVATE src)

if(MSVC)
  target_compile_options(mycc PRIVATE /W4)
else()
  target_compile_options(mycc PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
endif()

option(MYCC_SANITIZE "Enable AddressSanitizer + UBSan in Debug builds" ON)
if(MYCC_SANITIZE AND NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(mycc PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(mycc PRIVATE -fsanitize=address,undefined)
endif()
