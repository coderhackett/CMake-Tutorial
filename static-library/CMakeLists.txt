cmake_minimum_required(VERSION 3.5) #最低CMake版本

project(hello-lib) #工程名

# Create a lib
add_library(hello-lib STATIC
            src/hello.cpp)

# target_include_directories为一个目标（可能是一个库library也可能是可执行文件）添加头文件路径。
target_include_directories(hello-lib
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}/include)#设置这个可执行文件hello_headers需要包含的库的路径

#指定用那个源文件生成可执行程序
add_executable(hello-bin src/main.cpp)

#链接可执行程序和静态库
target_link_libraries(hello-bin
                     PRIVATE
                     hello-lib)