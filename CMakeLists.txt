cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# 设置项目名称
project(multithreading)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)

# 查找src目录下的所有.cpp文件
file(GLOB CPP_SOURCES src/*.cpp)

# 为每个源文件创建一个单独的项目
foreach(SOURCE_FILE ${CPP_SOURCES})
    # 获取文件名作为项目名（无扩展名）
    get_filename_component(PROJECT_NAME ${SOURCE_FILE} NAME_WE)

    # 添加可执行文件
    add_executable(${PROJECT_NAME} ${SOURCE_FILE})

    # 链接pthread库
    target_link_libraries(${PROJECT_NAME} pthread)
endforeach()
