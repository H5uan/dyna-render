file(GLOB_RECURSE HW7_SOURCE "*.cpp")
file(GLOB_RECURSE HW7_HEADER "*.h")

add_executable(HW7 ${HW7_SOURCE} ${HW7_HEADER})
target_link_libraries(HW7 GL-CORE)
target_include_directories(HW7 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
