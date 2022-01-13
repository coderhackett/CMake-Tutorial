----

title: CMake实战一：单个源文件

categories:[实战一]

tags:[CMake]

date: 2021/12/23

<div align = 'right'>作者：hackett</div>

<div align = 'right'>微信公众号：加班猿</div>

----



CMake 支持大写、小写和大小写混合命令。

在 linux 平台下使用 CMake 生成 Makefile 并编译的流程如下：

1. 编写 CMake 配置文件 CMakeLists.txt 。
2. 执行命令 `cmake PATH` 或者 `ccmake PATH` 生成 Makefile（`ccmake` 和 `cmake` 的区别在于前者提供了一个交互式的界面）。其中， `PATH` 是 CMakeLists.txt 所在的目录。
3. 使用 `make` 命令进行编译。
4. 编译生成可执行程序运行

### 1、创建目录

```shell
mkdir cmake
cd cmake
mkdir demo1
cd demo1
```

### 2、准备好需要编译的文件

这里做个演示  所以就来个简单的代码，计算两个数的和，源文件为`main.cpp`

```cpp
#include <stdio.h>
#include <stdlib.h>

int add(int a, int b) {
    return (a + b);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s argv[1] argv[2] \n", argv[0]);
        return 1;
    }
    int a = atof(argv[1]);
    int b = atoi(argv[2]);
    int result = add(a, b);
    printf("%d + %d = %d\n", a, b, result);
    return 0;
}
```

### 3、编写CMakeLists.txt

编写 CMakeLists.txt 文件，并保存在与`main.cpp` 源文件同个目录下：

```shell
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (demo1)

# 指定生成目标
add_executable(demo main.cpp)
```

CMakeLists.txt 的语法比较简单，由命令、注释和空格组成，其中命令是不区分大小写的。符号 `#` 后面的内容被认为是注释。命令由命令名称、小括号和参数组成，参数之间使用空格进行间隔。

对于上面的 CMakeLists.txt 文件，依次出现了几个命令：

1. `cmake_minimum_required`：指定运行此配置文件所需的 CMake 的最低版本；
2. `project`：参数值是 `demo1`，该命令表示项目的名称是 `demo1` 。
3. `add_executable`： 将名为`main.cpp` 的源文件编译成一个名称为 demo 的可执行文件。

### 4、编译项目

在`main.cpp`当前目录下新建一个`build`目录，进入`build`目录执行`cmake ..`,得到Makefile后再使用`make`命令编译得到demo可执行文件

新建build目录是方便我们清理cmake产生的缓存文件，不需要的时候直接删除`build`目录即可

```shell
[root@hackett build]# cmake ..
-- The C compiler identification is GNU 8.4.1
-- The CXX compiler identification is GNU 8.4.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /root/workspace/cmake/demo1/build
[root@hackett build]# ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  Makefile
[root@hackett build]# make
[ 50%] Building CXX object CMakeFiles/demo.dir/main.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo
[root@hackett build]# ls
CMakeCache.txt  CMakeFiles  cmake_install.cmake  demo  Makefile
[root@hackett build]# ./demo 2 3
2 + 3 is 5
```

如果你觉得文章还不错，可以给个"**三连**"，文章同步到个人微信公众号[**加班猿**]

我是**hackett**，我们下期见

参考文档：

[CMake入门实战](https://www.hahack.com/codes/cmake)

[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

