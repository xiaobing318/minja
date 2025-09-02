// test_cases.cpp - 测试用例实现
#include <iostream>
#include <string>
#include <minja/minja.hpp>
#include "test_helper.h"

// 全局模板路径
const std::string TEMPLATE_PATH = "templates/openai-gpt-oss-120b.jinja";

// 执行单个测试的辅助函数
TestResult run_single_test(const std::string& test_name,
                          const json& data,
                          const std::string& expected_output) {
    TestResult result;
    result.test_name = test_name;

    try {
        // 读取模板文件
        std::string template_str = read_template_file(TEMPLATE_PATH);

        // 创建minja环境并解析模板
        minja::Environment env;
        // 设置环境选项：保留尾部换行，去除块前后空白
        env.set_trim_blocks(true);      // 去除块标签后的第一个换行
        env.set_lstrip_blocks(true);    // 去除块标签前的空白

        auto tmpl = env.parse(template_str);

        // 渲染模板
        result.actual_output = env.render(tmpl, data);

        // 比较输出
        result.passed = compare_outputs(result.actual_output, expected_output);
        result.expected_output = expected_output;

        if (!result.passed) {
            result.error_message = "输出不匹配";
        }

    } catch (const std::exception& e) {
        result.passed = false;
        result.error_message = std::string("异常: ") + e.what();
    }

    print_test_result(result);
    return result;
}

// 测试1：基础功能测试
void run_basic_tests(TestStats& stats) {
    // 测试1.1：空消息列表
    {
        json data;
        data["messages"] = json::array();
        data["add_generation_prompt"] = false;

        // 空消息列表应该自动添加默认系统消息
        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("空消息列表（自动添加系统消息）", data, expected);
        stats.add_result(result);
    }

    // 测试1.2：带生成提示的空消息
    {
        json data;
        data["messages"] = json::array();
        data["add_generation_prompt"] = true;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n";

        TestResult result = run_single_test("空消息+生成提示", data, expected);
        stats.add_result(result);
    }
}

// 测试2：消息处理测试
void run_message_tests(TestStats& stats) {
    // 测试2.1：单个用户消息
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "Hello, how are you?")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Hello, how are you?\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("单个用户消息", data, expected);
        stats.add_result(result);
    }

    // 测试2.2：用户-助手对话
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "What is 2+2?"),
            create_message("assistant", "2+2 equals 4.")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "What is 2+2?\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "2+2 equals 4.\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("用户-助手对话", data, expected);
        stats.add_result(result);
    }

    // 测试2.3：多轮对话
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "Tell me a joke"),
            create_message("assistant", "Why did the chicken cross the road?"),
            create_message("user", "I don't know, why?"),
            create_message("assistant", "To get to the other side!")
        });
        data["add_generation_prompt"] = true;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Tell me a joke\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "Why did the chicken cross the road?\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "I don't know, why?\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "To get to the other side!\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n";

        TestResult result = run_single_test("多轮对话+生成提示", data, expected);
        stats.add_result(result);
    }
}

// 测试3：系统消息测试
void run_system_message_tests(TestStats& stats) {
    // 测试3.1：自定义系统消息
    {
        json data;
        data["messages"] = create_messages({
            create_message("system", "You are a Python programming expert."),
            create_message("user", "How do I read a file?")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a Python programming expert.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "How do I read a file?\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("自定义系统消息", data, expected);
        stats.add_result(result);
    }

    // 测试3.2：多个系统消息（测试只使用第一个）
    {
        json data;
        data["messages"] = create_messages({
            create_message("system", "System message 1"),
            create_message("system", "System message 2"),
            create_message("user", "Hello")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "System message 1\n"
            "<|im_end|>\n"
            "<|im_start|>system\n"
            "System message 2\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Hello\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("多个系统消息", data, expected);
        stats.add_result(result);
    }
}

// 测试4：工具消息测试
void run_tool_message_tests(TestStats& stats) {
    // 测试4.1：基本工具消息
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "What's the weather?"),
            create_message("tool", "Temperature: 22°C, Sunny")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "What's the weather?\n"
            "<|im_end|>\n"
            "<|im_start|>tool\n"
            "Temperature: 22°C, Sunny\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("基本工具消息", data, expected);
        stats.add_result(result);
    }

    // 测试4.2：带名称的工具消息
    {
        json data;
        json messages = json::array();

        // 创建带名称的工具消息
        json tool_msg;
        tool_msg["role"] = "tool";
        tool_msg["content"] = "Current temperature is 25°C";
        tool_msg["name"] = "weather_api";

        messages.push_back(create_message("user", "Check the weather"));
        messages.push_back(tool_msg);

        data["messages"] = messages;
        data["add_generation_prompt"] = true;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Check the weather\n"
            "<|im_end|>\n"
            "<|im_start|>tool\n"
            "Tool: weather_api\n"
            "Current temperature is 25°C\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n";

        TestResult result = run_single_test("带名称的工具消息", data, expected);
        stats.add_result(result);
    }
}

// 测试5：生成提示测试
void run_generation_prompt_tests(TestStats& stats) {
    // 测试5.1：不添加生成提示
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "Hello")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Hello\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("不添加生成提示", data, expected);
        stats.add_result(result);
    }

    // 测试5.2：添加生成提示
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "Hello")
        });
        data["add_generation_prompt"] = true;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Hello\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n";

        TestResult result = run_single_test("添加生成提示", data, expected);
        stats.add_result(result);
    }

    // 测试5.3：未定义add_generation_prompt（应该默认为false）
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "Test")
        });
        // 不设置 add_generation_prompt

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Test\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("未定义生成提示标志", data, expected);
        stats.add_result(result);
    }
}

// 测试6：边界情况测试
void run_edge_case_tests(TestStats& stats) {
    // 测试6.1：包含特殊字符的消息
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "What is <|im_start|>?"),
            create_message("assistant", "It's a special token: <|im_end|>")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "What is <|im_start|>?\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "It's a special token: <|im_end|>\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("包含特殊标记的消息", data, expected);
        stats.add_result(result);
    }

    // 测试6.2：多行消息
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "Line 1\nLine 2\nLine 3"),
            create_message("assistant", "Response\nwith\nmultiple\nlines")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Line 1\nLine 2\nLine 3\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "Response\nwith\nmultiple\nlines\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("多行消息", data, expected);
        stats.add_result(result);
    }

    // 测试6.3：Unicode字符
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", "你好，世界！🌍"),
            create_message("assistant", "Hello! 😊 こんにちは")
        });
        data["add_generation_prompt"] = true;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "你好，世界！🌍\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "Hello! 😊 こんにちは\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n";

        TestResult result = run_single_test("Unicode字符支持", data, expected);
        stats.add_result(result);
    }

    // 测试6.4：空内容消息
    {
        json data;
        data["messages"] = create_messages({
            create_message("user", ""),
            create_message("assistant", "")
        });
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("空内容消息", data, expected);
        stats.add_result(result);
    }

    // 测试6.5：未知角色（应该被忽略）
    {
        json data;
        json messages = json::array();
        messages.push_back(create_message("user", "Hello"));

        // 创建未知角色的消息
        json unknown_msg;
        unknown_msg["role"] = "unknown_role";
        unknown_msg["content"] = "This should be ignored";
        messages.push_back(unknown_msg);

        messages.push_back(create_message("assistant", "Hi there"));

        data["messages"] = messages;
        data["add_generation_prompt"] = false;

        std::string expected =
            "<|im_start|>system\n"
            "You are a helpful assistant.\n"
            "<|im_end|>\n"
            "<|im_start|>user\n"
            "Hello\n"
            "<|im_end|>\n"
            "<|im_start|>assistant\n"
            "Hi there\n"
            "<|im_end|>\n";

        TestResult result = run_single_test("未知角色消息（应被忽略）", data, expected);
        stats.add_result(result);
    }
}
