cmake_minimum_required(VERSION 3.10)

# set the project name
project(demo3)

aux_source_directory(. DIR_SRCS)

add_subdirectory(math)


# add the executable
add_executable(demo main.cpp)


target_link_libraries(demo MathFunctions)
