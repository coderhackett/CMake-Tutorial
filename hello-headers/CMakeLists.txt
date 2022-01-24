cmake_minimum_required(VERSION 3.5) #最低CMake版本

project(hello-headers) #工程名

set(SOURCES src/hello.cpp src/main.cpp) #创建一个变量，名字叫SOURCES。它包含了src下的hello.cpp、main.cpp文件

add_executable(hello-headers ${SOURCES}) #用所有的源文件生成一个可执行文件hello-headers，等价于命令：add_executable(hello_headers src/Hello.cpp src/main.cpp)

target_include_directories(hello-headers 
                           PRIVATE
                           ${PROJECT_SOURCE_DIR}/include)#设置这个可执行文件hello_headers需要包含的库的路径