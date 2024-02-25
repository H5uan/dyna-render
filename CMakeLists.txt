cmake_minimum_required(VERSION 3.15)

set(VCPKG_TOOLCHAIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake")

if (EXISTS "${VCPKG_TOOLCHAIN_PATH}")
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_TOOLCHAIN_PATH}" CACHE STRING "Vcpkg toolchain file")
    message(STATUS "Vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
else ()
    set(CMAKE_TOOLCHAIN_FILE "" CACHE STRING "Vcpkg toolchain file")
    message(STATUS "Using system vcpkg toolchain file")
    message(STATUS "Vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()
project(6610-project VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

# --- Output setting

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,HW6/Debug,HW6/$<CONFIG>>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,HW6/Debug,HW6/$<CONFIG>>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,HW6/Debug,HW6/$<CONFIG>>)

# --- Dependencies Searching
find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Stb REQUIRED)

# --- global include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/)

add_subdirectory(GL-Core)
add_subdirectory(HW6)










