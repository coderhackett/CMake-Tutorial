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
#add_test (test_run demo 3 2)

#add_test (test_35_2 demo 35 2)
#set_tests_properties (test_35_2 PROPERTIES PASS_REGULAR_EXPRESSION "37")

#add_test (test_5_2 demo 5 2)
#set_tests_properties (test_5_2 PROPERTIES PASS_REGULAR_EXPRESSION "7")

#add_test (test_2_3 demo 2 3)
#set_tests_properties (test_2_3 PROPERTIES PASS_REGULAR_EXPRESSION "5")

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


