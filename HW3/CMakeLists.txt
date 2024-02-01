file(GLOB_RECURSE HW2_SOURCE "*.cpp")
file(GLOB_RECURSE HW2_HEADER "*.h")

add_executable(HW2 ${HW2_SOURCE} ${HW2_HEADER})
target_link_libraries(HW2 GL-CORE)
target_include_directories(HW2 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
