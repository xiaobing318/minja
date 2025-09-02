# Jinja 模板测试项目

这是一个用于学习和测试 llama.cpp 项目中聊天模板的 C++ 测试程序，目前针对的是`openai-gpt-oss-120b.jinja`模板文件。项目使用Google的[minja](https://github.com/google/minja)库来解析和渲染 Jinja2 模板，下列是目前支持测试的模板文件列表：
 - openai-gpt-oss-120b.jinja

## 项目结构

```
jinja-template-test/
├── CMakeLists.txt           # CMake构建配置
├── src/
│   ├── main.cpp            # 主程序入口
│   └── test_cases.cpp      # 测试用例实现
├── templates/
│   └── openai-gpt-oss-120b.jinja  # Jinja模板文件
├── include/
│   └── test_helper.h       # 测试辅助函数
└── README.md              # 本文档
```

## 功能特性

- **完整的测试覆盖**：包含6大类测试，共20+个测试用例
- **彩色输出**：使用ANSI颜色代码美化终端输出
- **详细的中文注释**：每个函数和重要代码段都有中文注释
- **自动依赖管理**：使用CMake FetchContent自动下载依赖

## 测试类别

1. **基础功能测试**
   - 空消息列表处理
   - 自动添加默认系统消息
   - 生成提示符处理

2. **消息处理测试**
   - 单个用户消息
   - 用户-助手对话
   - 多轮对话

3. **系统消息测试**
   - 自定义系统消息
   - 多个系统消息处理

4. **工具消息测试**
   - 基本工具消息
   - 带名称的工具消息

5. **生成提示测试**
   - 控制是否添加生成提示
   - 未定义标志的默认行为

6. **边界情况测试**
   - 特殊字符处理
   - 多行消息
   - Unicode字符支持
   - 空内容消息
   - 未知角色处理

## 环境要求

- C++17或更高版本
- CMake 3.14或更高版本
- Git（用于下载依赖）
- 支持UTF-8的终端（用于显示中文和emoji）

## 快速开始

### 1. 克隆或创建项目

```bash
# 创建项目目录
mkdir jinja-template-test
cd jinja-template-test

# 创建目录结构
mkdir src templates include build
```

### 2. 复制文件

将上述提供的所有文件复制到相应目录中：
- `CMakeLists.txt` → 根目录
- `main.cpp` → `src/`
- `test_cases.cpp` → `src/`
- `test_helper.h` → `include/`
- `openai-gpt-oss-120b.jinja` → `templates/`

### 3. 构建项目

```bash
# 进入构建目录
cd build

# 配置项目（会自动下载依赖）
cmake ..

# 编译项目
cmake --build .

# 或者使用make（Linux/Mac）
make -j4
```

### 4. 运行测试

```bash
# 在build目录中运行
./jinja_test

# Windows下
jinja_test.exe
```

## 输出示例

```
============================================================
     Minja/Jinja 模板测试程序
     测试模板: openai-gpt-oss-120b.jinja
============================================================

检查minja库状态...
✓ Minja库工作正常

=== 基础功能测试 ===
[通过] 空消息列表（自动添加系统消息）
[通过] 空消息+生成提示

=== 消息处理测试 ===
[通过] 单个用户消息
[通过] 用户-助手对话
[通过] 多轮对话+生成提示

...

==================================================
测试总结
==================================================
通过: 20/20 (100%)
==================================================
```

## Jinja模板语法说明

### 1. 变量输出
```jinja
{{ variable }}  # 输出变量值
{{ message['content'] }}  # 访问字典元素
```

### 2. 控制结构
```jinja
{%- if condition -%}
    # 条件为真时的内容
{%- elif other_condition -%}
    # 其他条件
{%- else -%}
    # 默认情况
{%- endif -%}
```

### 3. 循环
```jinja
{%- for item in items -%}
    {{ item }}
{%- endfor -%}
```

### 4. 空白控制
- `{%-` 和 `-%}`: 删除标签前后的空白
- `trim_blocks`: 删除块后的第一个换行
- `lstrip_blocks`: 删除块前的空白

### 5. 命名空间
```jinja
{%- set ns = namespace(found=false) -%}
```

## minja C++ API 说明

### 1. 环境创建
```cpp
minja::Environment env;
env.set_trim_blocks(true);     // 设置空白处理
env.set_lstrip_blocks(true);
```

### 2. 模板解析
```cpp
auto tmpl = env.parse(template_string);
```

### 3. 模板渲染
```cpp
json data;  // 使用nlohmann::json
std::string result = env.render(tmpl, data);
```

## 故障排除

### 1. CMake找不到编译器
确保已安装C++编译器（GCC、Clang或MSVC）

### 2. 下载依赖失败
检查网络连接，或手动下载minja和nlohmann/json

### 3. 中文显示乱码
- Windows: 使用 `chcp 65001` 切换到UTF-8
- Linux/Mac: 确保终端使用UTF-8编码

### 4. 链接错误
确保CMake版本>=3.14，支持FetchContent

## 扩展建议

1. **添加更多模板**：可以测试其他聊天模板格式
2. **性能测试**：添加大量消息的性能测试
3. **模板验证**：添加模板语法验证功能
4. **JSON Schema验证**：验证输入数据格式
5. **命令行参数**：支持指定不同的模板文件

## 参考资源

- [minja GitHub](https://github.com/google/minja)
- [Jinja2 文档](https://jinja.palletsprojects.com/)
- [llama.cpp](https://github.com/ggml-org/llama.cpp)
- [nlohmann/json](https://github.com/nlohmann/json)

## 许可证

本测试项目仅用于学习目的。
