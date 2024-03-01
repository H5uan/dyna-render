file(GLOB_RECURSE HW6_SOURCE "*.cpp")
file(GLOB_RECURSE HW6_HEADER "*.h")

add_executable(HW6 ${HW6_SOURCE} ${HW6_HEADER})
target_link_libraries(HW6 GL-CORE)
target_include_directories(HW6 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
