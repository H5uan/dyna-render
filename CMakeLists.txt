cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
project(6610-project VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

file(GLOB_RECURSE GL_CORE_SOURCE "${PROJECT_SOURCE_DIR}/GL-Core/*.cpp")
file(GLOB_RECURSE GL_CORE_HEADER "${PROJECT_SOURCE_DIR}/GL-Core/*.h")

add_library(GL-CORE STATIC ${GL_CORE_SOURCE} ${GL_CORE_HEADER})
target_link_libraries(GL-CORE
        glad::glad
        glfw
        imgui::imgui
        spdlog::spdlog)
target_include_directories(GL-CORE PUBLIC ${PROJECT_SOURCE_DIR}/GL-Core/src)


add_subdirectory(HW1)








