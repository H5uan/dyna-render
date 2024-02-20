file(GLOB_RECURSE HW5_SOURCE "*.cpp")
file(GLOB_RECURSE HW5_HEADER "*.h")

add_executable(HW5 ${HW5_SOURCE} ${HW5_HEADER})
target_link_libraries(HW5 GL-CORE)
target_include_directories(HW5 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
