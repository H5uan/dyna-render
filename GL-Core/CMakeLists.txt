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