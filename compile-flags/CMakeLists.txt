cmake_minimum_required(VERSION 3.5) #最低CMake版本

project(compile-flags) #工程名

#强制设置默认C++编译标志变量为缓存变量，如CMake（五） build type所说，该缓存变量被定义在文件中，相当于全局变量，源文件中也可以使用这个变量
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEX2" CACHE STRING "Set C++ Compile Flags" FORCE)

add_executable(cmake-example-compile-flags main.cpp)

#为可执行文件添加私有编译定义
target_compile_definitions(cmake-example-compile-flags
                            PRIVATE EX3)


