----

title: CMake实战四：安装测试和添加环境生成安装包

categories:[实战四]

tags:[CMake]

date: 2021/12/24

<div align = 'right'>作者：hackett</div>

<div align = 'right'>微信公众号：加班猿</div>

----

## 1、安装测试

CMake 也可以指定安装规则，以及添加测试。这两个功能分别可以通过在产生 Makefile 后使用 `make install` 和 `make test` 来执行。在 GNU Makefile 里，你可能需要为此编写 `install` 和 `test` 两个伪目标和相应的规则，但在 CMake 里，这样的工作同样只需要简单的调用几条命令。

### 1.1定制安装规则

首先先在 math/CMakeLists.txt 文件里添加下面两行：

```
# 指定 MathFunctions 库的安装路径
install (TARGETS MathFunctions DESTINATION bin)
install (FILES MathFunctions.h DESTINATION include)
```

指明 MathFunctions 库的安装路径。之后同样修改根目录的 CMakeLists 文件，在末尾添加下面几行：

```
# 指定安装路径
install (TARGETS Demo DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h"
         DESTINATION include)
```

通过上面的定制，生成的 Demo 文件和 MathFunctions 函数库 libMathFunctions.o 文件将会被复制到 `/usr/local/bin` 中，而 MathFunctions.h 和生成的 config.h 文件则会被复制到 `/usr/local/include` 中。我们可以验证一下（顺带一提的是，这里的 `/usr/local/` 是默认安装到的根目录，可以通过修改 `CMAKE_INSTALL_PREFIX` 变量的值来指定这些文件应该拷贝到哪个根目录）：

```shell
[root@hackett demo5]# make install
Consolidate compiler generated dependencies of target MathFunctions
[ 50%] Built target MathFunctions
Consolidate compiler generated dependencies of target demo
[100%] Built target demo
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/bin/demo
-- Installing: /usr/local/include/config.h
-- Installing: /usr/local/bin/libMathFunctions.a
-- Installing: /usr/local/include/myMath.h
[root@hackett demo5]# ls /usr/local/bin/
demo     	libMathFunctions.a               
[root@iZwz97bu0gr8vx0j8l6kkzZ demo5]# ls /usr/local/include/
config.h	myMath.h
```

### 1.2工程添加测试

添加测试同样很简单。CMake 提供了一个称为 CTest 的测试工具。我们要做的只是在项目根目录的 CMakeLists 文件中调用一系列的 `add_test` 命令。

CMakeLists.txt

```shell
cmake_minimum_required(VERSION 3.10)
# set the project name
project(demo5)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)
# 是否使用自己的 MathFunctions 库
option (USE_MYMATH
        "Use provided math implementation" ON)

# 是否加入 MathFunctions 库
if (USE_MYMATH)
    include_directories ("${PROJECT_SOURCE_DIR}/math")
    add_subdirectory (math)
    set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)

aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(demo ${DIR_SRCS})
target_link_libraries(demo ${EXTRA_LIBS})

# 指定安装路径
install (TARGETS demo DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h"
                 DESTINATION include)

enable_testing()
# 测试程序是否成功运行
add_test (test_run demo 3 2)

add_test (test_35_2 demo 35 2)
set_tests_properties (test_35_2 PROPERTIES PASS_REGULAR_EXPRESSION "37")

add_test (test_5_2 demo 5 2)
set_tests_properties (test_5_2 PROPERTIES PASS_REGULAR_EXPRESSION "7")

add_test (test_2_3 demo 2 3)
set_tests_properties (test_2_3 PROPERTIES PASS_REGULAR_EXPRESSION "5")
```

上面的代码包含了四个测试。第一个测试 `test_run` 用来测试程序是否成功运行并返回 0 值。剩下的三个测试分别用来测试 35 + 2 、5 + 2、2 + 3是否都能得到正确的结果。其中 `PASS_REGULAR_EXPRESSION` 用来测试输出是否包含后面跟着的字符串。

测试结果：

```shell
[root@hackett demo5]# make 
Consolidate compiler generated dependencies of target MathFunctions
[ 50%] Built target MathFunctions
Consolidate compiler generated dependencies of target demo
[ 75%] Building CXX object CMakeFiles/demo.dir/main.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo
[root@hackett demo5]# make test
Running tests...
Test project /root/workspace/cmake/demo5
    Start 1: test_run
1/4 Test #1: test_run .........................   Passed    0.00 sec
    Start 2: test_35_2
2/4 Test #2: test_35_2 ........................   Passed    0.00 sec
    Start 3: test_5_2
3/4 Test #3: test_5_2 .........................   Passed    0.00 sec
    Start 4: test_2_3
4/4 Test #4: test_2_3 .........................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 4

Total Test time (real) =   0.01 sec
```

如果要测试更多的输入数据，像上面那样一个个写测试用例未免太繁琐。这时可以通过编写宏来实现：

```shell
# 定义一个宏，用来简化测试工作
macro (do_test arg1 arg2 result)
  add_test (test_${arg1}_${arg2} demo ${arg1} ${arg2})
  set_tests_properties (test_${arg1}_${arg2}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)
 
# 使用该宏进行一系列的数据测试
do_test (35 2 "37")
do_test (5 52 "7")
do_test (2 3 "5")
```

关于 CTest 的更详细的用法可以通过 `man 1 ctest` 参考 CTest 的文档。

## 2、添加版本号

首先修改顶层 CMakeLists 文件，在 `project` 命令之后加入如下两行：

```shell
set (Demo_VERSION_MAJOR 1)
set (Demo_VERSION_MINOR 0)
```

分别指定当前的项目的主版本号和副版本号。

之后，为了在代码中获取版本信息，我们可以修改 [config.h.in](http://config.h.in/) 文件，添加两个预定义变量：

```shell
// the configured options and settings for Tutorial
#define Demo_VERSION_MAJOR @Demo_VERSION_MAJOR@
#define Demo_VERSION_MINOR @Demo_VERSION_MINOR@
```

这样就可以直接在代码中打印版本信息了：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#ifdef USE_MYMATH
  #include "math/myMath.h"
#else
  #include <math.h>
#endif
int sub(int a, int b) {
	return (a - b);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s argv[1] argv[2] \n", argv[0]);
        return 1;
    }
    printf("version : %d.%d\n", Demo_VERSION_MAJOR, Demo_VERSION_MINOR);
    int a = atof(argv[1]);
    int b = atoi(argv[2]);

#ifdef USE_MYMATH
    printf("Now we use our own Math library. \n");
    int result = add(a, b);
    printf("%d + %d = %d\n", a, b, result);
#else
    printf("Now we use the main.cpp sub function. \n");
    int result = sub(a, b);
    printf("%d - %d = %d\n", a, b, result);
#endif
    return 0;
}
```

运行结果：

```shell
[root@hackett demo7]# cmake .
-- Configuring done
-- Generating done
-- Build files have been written to: /root/workspace/cmake/demo7
[root@hackett demo7]# make
Consolidate compiler generated dependencies of target MathFunctions
[ 25%] Building CXX object math/CMakeFiles/MathFunctions.dir/myMath.cpp.o
[ 50%] Linking CXX static library libMathFunctions.a
[ 50%] Built target MathFunctions
[ 75%] Building CXX object CMakeFiles/demo.dir/main.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo
[root@hackett demo7]# ./demo 2 3 
version : 1.0
Now we use our own Math library. 
2 + 3 = 5
```

## 3、生成安装包

如何配置生成各种平台上的安装包，包括二进制安装包和源码安装包。为了完成这个任务，我们需要用到 CPack ，它同样也是由 CMake 提供的一个工具，专门用于打包。

首先在顶层的 CMakeLists.txt 文件尾部添加下面几行：

```shell
# 构建一个 CPack 安装包
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE
  "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Demo_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Demo_VERSION_MINOR}")
include (CPack)
```

上面的代码做了以下几个工作：

1. 导入 InstallRequiredSystemLibraries 模块，以便之后导入 CPack 模块；
2. 设置一些 CPack 相关变量，包括版权信息和版本信息，其中版本信息用了上一节定义的版本号；
3. 导入 CPack 模块。

创建一个``License.txt`文件

```shell
touch License.txt
```

接下来的工作是像往常一样构建工程，并执行 `cpack` 命令。

- 生成二进制安装包：

```shell
cpack -C CPackConfig.cmake
```

- 生成源码安装包

```shell
cpack -C CPackSourceConfig.cmake
```

我们可以试一下。在生成项目后，执行 `cpack -C CPackConfig.cmake` 命令：

```shell
[root@hackett demo8]# cmake .
-- Configuring done
-- Generating done
-- Build files have been written to: /root/workspace/cmake/demo8
[root@hackett demo8]# cpack -C CPackConfig.cmake
CPack: Create package using STGZ
CPack: Install projects
CPack: - Run preinstall target for: demo8
CPack: - Install project: demo8 [CPackConfig.cmake]
CPack: Create package
CPack: - package: /root/workspace/cmake/demo8/demo8-1.0.1-Linux.sh generated.
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: demo8
CPack: - Install project: demo8 [CPackConfig.cmake]
CPack: Create package
CPack: - package: /root/workspace/cmake/demo8/demo8-1.0.1-Linux.tar.gz generated.
CPack: Create package using TZ
CPack: Install projects
CPack: - Run preinstall target for: demo8
CPack: - Install project: demo8 [CPackConfig.cmake]
CPack: Create package
CPack: - package: /root/workspace/cmake/demo8/demo8-1.0.1-Linux.tar.Z generated.
[root@hackett demo8]# ls
CMakeCache.txt  cmake_install.cmake  config.h     CPackConfig.cmake  CPackSourceConfig.cmake  demo                  demo8-1.0.1-Linux.tar.gz  install_manifest.txt  main.cpp  math
CMakeFiles      CMakeLists.txt       config.h.in  _CPack_Packages    CTestTestfile.cmake      demo8-1.0.1-Linux.sh  demo8-1.0.1-Linux.tar.Z   License.txt           Makefile
```

这 3 个二进制包文件所包含的内容是完全相同的。我们可以执行其中一个。此时会出现一个由 CPack 自动生成的交互式安装界面：

```shell
[root@hackett demo8]# sh demo8-1.0.1-Linux.sh 
demo8 Installer Version: 1.0.1, Copyright (c) Humanity
This is a self-extracting archive.
The archive will be extracted to: /root/workspace/cmake/demo8

If you want to stop extracting, please press <ctrl-C>.


Do you accept the license? [yn]: 
y
By default the demo8 will be installed in:
  "/root/workspace/cmake/demo8/demo8-1.0.1-Linux"
Do you want to include the subdirectory demo8-1.0.1-Linux?
Saying no will install in: "/root/workspace/cmake/demo8" [Yn]: 
y

Using target directory: /root/workspace/cmake/demo8/demo8-1.0.1-Linux
Extracting, please wait...

Unpacking finished successfully
```

完成后提示安装到了 demo8-1.0.1-Linux 子目录中，我们可以进去执行该程序：

```shell
[root@hackett demo8]# ls
CMakeCache.txt  cmake_install.cmake  config.h     CPackConfig.cmake  CPackSourceConfig.cmake  demo               demo8-1.0.1-Linux.sh      demo8-1.0.1-Linux.tar.Z  main.cpp  math
CMakeFiles      CMakeLists.txt       config.h.in  _CPack_Packages    CTestTestfile.cmake      demo8-1.0.1-Linux  demo8-1.0.1-Linux.tar.gz  install_manifest.txt     Makefile
[root@hackett demo8]# ./demo8-1.0.1-Linux/bin/demo 3 2
version : 1.0
Now we use our own Math library. 
3 + 2 = 5
```

如果你觉得文章还不错，可以给个"**三连**"，文章同步到个人微信公众号[**加班猿**]

我是**hackett**，我们下期见

参考文档：

[CMake入门实战](https://www.hahack.com/codes/cmake)

[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)











