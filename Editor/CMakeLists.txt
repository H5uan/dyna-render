 file(GLOB_RECURSE HW8_SOURCE "*.cpp")
file(GLOB_RECURSE HW8_HEADER "*.h")

add_executable(HW8 ${HW8_SOURCE} ${HW8_HEADER})
target_link_libraries(HW8 GL-CORE)
target_include_directories(HW8 PRIVATE ${PROJECT_SOURCE_DIR}/GL-Core/src)
