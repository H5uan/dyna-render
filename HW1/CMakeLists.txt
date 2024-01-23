file(GLOB_RECURSE HW1_SOURCE "*.cpp")
file(GLOB_RECURSE HW1_HEADER "*.h")

file(GLOB_RECURSE GL_CORE_SOURCE "../GL-Core/*.cpp")
file(GLOB_RECURSE GL_CORE_HEADER "../GL-Core/*.h")

add_executable(HW1
        ${HW1_SOURCE}
        ${HW1_HEADER}
)
target_link_libraries(HW1 GL-CORE)
target_include_directories(HW1 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
