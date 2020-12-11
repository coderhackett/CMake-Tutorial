# CMake-Tutorial
Make 工具有好几种：GNU Make、qmake、pmake等
这些 Make 工具遵循着不同的规范和标准，所执行的 Makefile 格式也千差万别
CMake它首先允许开发者编写一种平台无关的 CMakeList.txt 文件来定制整个编译流程，然后再根据目标用户的平台进一步生成所需的本地化 Makefile 和工程文件，如 Unix 的 Makefile 或 Windows 的 Visual Studio 工程。
使用CMake生成Makefile并编译的流程：
1、编写 CMake 配置文件 CMakeLists.txt 。
2、执行命令 cmake PATH 或者 ccmake PATH 生成 Makefile（ccmake 和 cmake 的区别在于前者提供了一个交互式的界面）。其中， PATH 是 CMakeLists.txt 所在的目录。
3、使用 make 命令进行编译。

参考链接：
https://cmake.org/cmake/help/latest/guide/tutorial/index.html
https://gitlab.kitware.com/cmake/cmake/-/tree/master/Help/guide/tutorial
https://www.hahack.com/codes/cmake/
https://aiden-dong.github.io/2019/07/20/CMake%E6%95%99%E7%A8%8B%E4%B9%8BCMake%E4%BB%8E%E5%85%A5%E9%97%A8%E5%88%B0%E5%BA%94%E7%94%A8/
https://github.com/chaneyzorn/CMake-tutorial
