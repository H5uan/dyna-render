file(GLOB_RECURSE HW3_SOURCE "*.cpp")
file(GLOB_RECURSE HW3_HEADER "*.h")

add_executable(HW3 ${HW3_SOURCE} ${HW3_HEADER})
target_link_libraries(HW3 GL-CORE)
target_include_directories(HW3 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
