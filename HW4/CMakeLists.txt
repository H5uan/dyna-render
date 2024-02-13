file(GLOB_RECURSE HW4_SOURCE "*.cpp")
file(GLOB_RECURSE HW4_HEADER "*.h")

add_executable(HW4 ${HW4_SOURCE} ${HW4_HEADER})
target_link_libraries(HW4 GL-CORE)
target_include_directories(HW4 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
