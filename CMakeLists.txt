cmake_minimum_required(VERSION 3.15)

set(VCPKG_TOOLCHAIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake")

if(EXISTS "${VCPKG_TOOLCHAIN_PATH}")
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_TOOLCHAIN_PATH}" CACHE STRING "Vcpkg toolchain file")
else()
    set(CMAKE_TOOLCHAIN_FILE "" CACHE STRING "Vcpkg toolchain file")
    message(STATUS "Using system vcpkg toolchain file")
endif()
project(6610-project VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,HW2/Debug,HW2/$<CONFIG>>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,HW2/Debug,HW2/$<CONFIG>>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,HW2/Debug,HW2/$<CONFIG>>)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Stb REQUIRED)

file(GLOB_RECURSE GL_CORE_SOURCE "${PROJECT_SOURCE_DIR}/GL-Core/*.cpp")
file(GLOB_RECURSE GL_CORE_HEADER "${PROJECT_SOURCE_DIR}/GL-Core/*.h")

add_library(GL-CORE STATIC ${GL_CORE_SOURCE} ${GL_CORE_HEADER})
target_link_libraries(GL-CORE
        glad::glad
        glfw
        imgui::imgui
        spdlog::spdlog
        assimp::assimp
)
target_include_directories(GL-CORE PUBLIC ${Stb_INCLUDE_DIR})
target_include_directories(GL-CORE PUBLIC ${PROJECT_SOURCE_DIR}/GL-Core/src)

add_subdirectory(HW4)










