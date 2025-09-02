# CMakeLists.txt - 用于构建minja模板测试项目
cmake_minimum_required(VERSION 3.14)
project(JinjaTemplateTest)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加编译选项
if(MSVC)
    add_compile_options(/utf-8)  # Windows下支持UTF-8编码
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 使用FetchContent下载minja
include(FetchContent)
FetchContent_Declare(
    minja
    GIT_REPOSITORY https://github.com/google/minja.git
    GIT_TAG main  # 或者使用特定的版本标签
)
FetchContent_MakeAvailable(minja)

# 添加nlohmann/json库（minja的依赖）
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${minja_SOURCE_DIR}/include
)

# 源文件
set(SOURCES
    src/main.cpp
    src/test_cases.cpp
)

# 创建可执行文件
add_executable(jinja_test ${SOURCES})

# 链接库
target_link_libraries(jinja_test
    PRIVATE
    nlohmann_json::nlohmann_json
)

# 复制模板文件到构建目录
configure_file(
    ${CMAKE_SOURCE_DIR}/templates/openai-gpt-oss-120b.jinja
    ${CMAKE_BINARY_DIR}/templates/openai-gpt-oss-120b.jinja
    COPYONLY
)
