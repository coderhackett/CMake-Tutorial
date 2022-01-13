----

title: CMake实战二：多个源文件，同一或多个目录

categories:[实战二]

tags:[CMake]

date: 2021/12/23

<div align = 'right'>作者：hackett</div>

<div align = 'right'>微信公众号：加班猿</div>

----

## 

### 1、同一目录，多个源文件

CMake实战一只有单个源文件，现在把`add`函数写入myMath.cpp的源文件里面，声明放到`myMath.h`源文件里面

工程树状图如下：

```shell
demo2/
├── CMakeLists.txt
├── main.cpp
├── myMath.cpp
└── myMath.h
```

这个时候，CMakeLists.txt 可以改成如下的形式：

```
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo2)

# 指定生成目标
add_executable(Demo main.cpp myMath.cpp)
```

main.cpp

```cpp
#include <stdio.h>
#include <stdlib.h>
#include "myMath.h"
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

myMath.cpp

```cpp
#include "myMath.h"
int add(int a, int b) {
    return (a + b);
}
```

myMath.h

```cpp
int add(int a, int b);
```

唯一的改动只是在 `add_executable` 命令中增加了一个 `myMath.cpp` 源文件。这样写当然没什么问题，但是如果源文件很多，把所有源文件的名字都加进去将是一件烦人的工作。更省事的方法是使用 `aux_source_directory` 命令，该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名。其语法如下：

```shell
aux_source_directory(<dir> <variable>)
```

因此，可以修改 CMakeLists.txt 如下：

```shell
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (demo2)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(demo ${DIR_SRCS})
```

这样，CMake 会将当前目录所有源文件的文件名赋值给变量 `DIR_SRCS` ，再指示变量 `DIR_SRCS` 中的源文件需要编译成一个名称为 demo 的可执行文件。

新建build目录是方便我们清理cmake产生的缓存文件，不需要的时候直接删除`build`目录即可

```cpp
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
-- Build files have been written to: /root/workspace/cmake/demo2/build
[root@hackett build]# make
[ 33%] Building CXX object CMakeFiles/demo.dir/main.cpp.o
[ 66%] Building CXX object CMakeFiles/demo.dir/myMath.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo
[root@hackett build]# ./demo 2 3
2 + 3 is 5
```

### 2、多个目录，多个源文件

将 `myMath.h` 和 `myMath.cc` 文件移动到 `math `目录下

```shell
../demo3
├── CMakeLists.txt
├── main.cpp
└── math
    ├── myMath.cpp
    └── myMath.h
```

对于这种情况，需要分别在项目根目录 demo3 和 math 目录里各编写一个 CMakeLists.txt 文件。为了方便，我们可以先将 math 目录里的文件编译成静态库再由 main 函数调用。

根目录中的 CMakeLists.txt ：

```shell
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (demo3)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 添加 math 子目录
add_subdirectory(math)

# 指定生成目标 
add_executable(demo main.cpp)

# 添加链接库
target_link_libraries(demo MathFunctions)
```

该文件添加了下面的内容:  `add_subdirectory` 指明本项目包含一个子目录 math，这样 math 目录下的 CMakeLists.txt 文件和源代码也会被处理 。使用命令 `target_link_libraries` 指明可执行文件 main 需要连接一个名为 MathFunctions 的链接库 。

子目录中的 CMakeLists.txt：

```shell
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)

# 生成链接库
add_library (MathFunctions ${DIR_LIB_SRCS})
```

在该文件中使用命令 `add_library` 将 src 目录中的源文件编译为静态链接库。

【构建】——【编译】——【执行】

```shell
[root@hackett demo3]# mkdir build
[root@hackett demo3]# cd build/
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
-- Build files have been written to: /root/workspace/cmake/demo3/build
[root@hackett build]# make
[ 25%] Building CXX object math/CMakeFiles/MathFunctions.dir/myMath.cpp.o
[ 50%] Linking CXX static library libMathFunctions.a
[ 50%] Built target MathFunctions
[ 75%] Building CXX object CMakeFiles/demo.dir/main.cpp.o
[100%] Linking CXX executable demo
[100%] Built target demo
[root@hackett build]# ./demo 2 3 
2 + 3 is 5
```

如果你觉得文章还不错，可以给个"**三连**"，文章同步到个人微信公众号[**加班猿**]

我是**hackett**，我们下期见

参考文档：

[CMake入门实战](https://www.hahack.com/codes/cmake)

[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)