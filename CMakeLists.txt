cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(CubeSky VERSION 1.0)

# 添加 vcpkg 支持
set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "vcpkg triplet")
set(CMAKE_TOOLCHAIN_FILE "/root/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain file")

# 添加执行文件目标
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(CubeSky PRIVATE event)
target_link_libraries(CubeSky PRIVATE yyjson)
target_link_libraries(CubeSky PRIVATE mimalloc)
target_link_libraries(CubeSky PRIVATE toml)

# 自动包含 src 目录下的所有 .c 文件
file(GLOB_RECURSE SRC_FILES "src/*.c")
file(GLOB_RECURSE INCLUDE_FILES "include/*.h")

# 将源文件添加到目标
target_sources(${PROJECT_NAME} PRIVATE ${INCLUDE_FILES} ${SRC_FILES})

# 设置头文件的包含目录
target_include_directories(${PROJECT_NAME} PRIVATE include)

execute_process( COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR})
