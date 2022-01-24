cmake_minimum_required(VERSION 3.5) #最低CMake版本

project(third-party-lib) #工程名

#使用库文件系统和系统查找boost install
find_package(Boost 1.46.1 REQUIRED COMPONENTS filesystem system)

#这是第三方库，而不是自己生成的静态动态库
if(Boost_FOUND)
    message("boost found")
else()
    message(FATAL_ERROR "cann't find boost")
endif()


add_executable(third-party-lib main.cpp)

target_link_libraries(third-party-lib
                      PRIVATE
                      Boost::filesystem)


