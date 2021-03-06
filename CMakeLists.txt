cmake_minimum_required(VERSION 3.17)


#设定vckpg 路径
#SET(CMAKE_TOOLCHAIN_FILE, /to/your/vcpkgpath)


project(C11_Learning)

#设置自定义宏
#add_definitions(-DSTARTUP)


#设定CPP版本
set(CMAKE_CXX_STANDARD 11)

#设置UNICODE字符集
#add_definitions(-DUNICODE -D_UNICODE)

#编译模式
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

include_directories(include)
include_directories(dependencies/include)

link_directories(lib)
link_directories(dependencies/lib)


#扫描src目录, 获取.c .cpp 源码文件列表
file(GLOB_RECURSE DIR_SRCS ./src/main.cpp)

# 可执行文件输出目录
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/runtime)


#使用源码列表生成可执行文件
ADD_EXECUTABLE(learning ${DIR_SRCS})


ADD_EXECUTABLE(shell 
              src/shell.cpp
              src/config.cpp
              src/zbx_sender.cpp
              src/CFileReader.cpp
              )


target_link_libraries(shell spdlog.lib)

#设置msvc编译选项 -- 目标是否使用静态运行时 
IF(MSVC) 
  target_compile_options(learning PUBLIC /MT)
ENDIF()



