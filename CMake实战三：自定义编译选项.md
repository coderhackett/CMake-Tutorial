----



title: CMake实战三：自定义编译选项

categories:[实战三]

tags:[CMake]

date: 2021/12/24

<div align = 'right'>作者：hackett</div>

<div align = 'right'>微信公众号：加班猿</div>

----

## 

CMake 允许为项目增加编译选项，从而可以根据用户的环境和需求选择最合适的编译方案。

很多开源库都会有CMake来进行管理编译，比如亚马逊AWS的WebRTC中的CMake里面有这么一行

```shell
option(USE_OPENSSL "Use openssl as crypto library" ON)
```

`ON`表示使用openssl的库，`OFF`表示不使用openssl的库

### 1、修改CMakeLists文件

```shell
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (demo4)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# 是否使用自己的 MathFunctions 库
option (USE_MYMATH "Use provided math implementation" ON)

# 是否加入 MathFunctions 库
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/math")
  add_subdirectory (math)  
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(demo ${DIR_SRCS})
target_link_libraries (demo  ${EXTRA_LIBS})
```

其中：

1. `configure_file` 命令用于加入一个配置头文件 config.h ，这个文件由 CMake 从 [config.h.in](http://config.h.in/) 生成，通过这样的机制，将可以通过预定义一些参数和变量来控制代码的生成。
2.  `option` 命令添加了一个 `USE_MYMATH` 选项，并且默认值为 `ON` 。
3.  `USE_MYMATH` 变量的值来决定是否使用我们自己编写的 MathFunctions 库。

### 2、修改main.cpp文件

让其根据 `USE_MYMATH` 的预定义值来决定是否调用标准库还是 MathFunctions 库:

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
    if (argc < 3){
        printf("Usage: %s base exponent \n", argv[0]);
        return 1;
    }
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

### 3、编写config.h.in文件

上面的程序值得注意的是第2行，这里引用了一个 config.h 文件，这个文件预定义了 `USE_MYMATH` 的值。但我们并不直接编写这个文件，为了方便从 CMakeLists.txt 中导入配置，我们编写一个 [config.h.in](http://config.h.in/) 文件，内容如下：

```shell
#cmakedefine USE_MYMATH
```

这样 CMake 会自动根据 CMakeLists 配置文件中的设置自动生成 config.h 文件。

### 4、编译项目

`option (USE_MYMATH "Use provided math implementation" ON)`

运行结果：

```shell
[root@hackett demo4]# cmake .
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
-- Build files have been written to: /root/workspace/cmake/demo4
[root@hackett demo4]# cat config.h
#define USE_MYMATH
[root@hackett demo4]# make
Consolidate compiler generated dependencies of target MathFunctions
[ 50%] Built target MathFunctions
Consolidate compiler generated dependencies of target demo
[ 75%] Building CXX object CMakeFiles/demo.dir/main.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo
[root@hackett demo4]# ./demo 3 2
Now we use our own Math library. 
3 + 2 = 5
```

`option (USE_MYMATH "Use provided math implementation" OFF)`

运行结果：

```shell
[root@hackett demo4]# cmake .
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
-- Build files have been written to: /root/workspace/cmake/demo4
[root@hackett demo4]# cat config.h
/* #undef USE_MYMATH */
[root@hackett demo4]# make
[ 50%] Building CXX object CMakeFiles/demo.dir/main.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo
[root@hackett demo4]# ./demo  3 2
Now we use the main.cpp sub function. 
3 - 2 = 1
```

如果你觉得文章还不错，可以给个"**三连**"，文章同步到个人微信公众号[**加班猿**]

我是**hackett**，我们下期见

参考文档：

[CMake入门实战](https://www.hahack.com/codes/cmake)

[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)



















