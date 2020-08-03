cmake_minimum_required(VERSION 2.8)

project(C11_Learning)


#设定CPP版本
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE debug)

#编译模式
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

include_directories(include)

link_directories(lib)


#源码文件目录
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

# 可执行文件输出目录
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/runtime)


#使用源码列表生成可执行文件
ADD_EXECUTABLE(learning ${DIR_SRCS})





