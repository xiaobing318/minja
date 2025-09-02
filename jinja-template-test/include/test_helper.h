// test_helper.h - 测试辅助函数和工具
#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

// 使用nlohmann::json库
using json = nlohmann::json;

// 颜色输出宏定义（用于终端彩色输出）
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_RESET   "\033[0m"

// 测试结果结构体
struct TestResult {
    std::string test_name;      // 测试名称
    bool passed;                 // 是否通过
    std::string actual_output;   // 实际输出
    std::string expected_output; // 期望输出
    std::string error_message;   // 错误信息
};

// 辅助函数：读取模板文件
inline std::string read_template_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开模板文件: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 辅助函数：打印测试结果
inline void print_test_result(const TestResult& result) {
    if (result.passed) {
        std::cout << COLOR_GREEN << "[通过] " << COLOR_RESET;
    } else {
        std::cout << COLOR_RED << "[失败] " << COLOR_RESET;
    }

    std::cout << result.test_name << std::endl;

    if (!result.passed) {
        std::cout << COLOR_YELLOW << "  错误信息: " << COLOR_RESET
                  << result.error_message << std::endl;
        std::cout << COLOR_YELLOW << "  期望输出:\n" << COLOR_RESET
                  << result.expected_output << std::endl;
        std::cout << COLOR_YELLOW << "  实际输出:\n" << COLOR_RESET
                  << result.actual_output << std::endl;
    }
}

// 辅助函数：创建消息JSON对象
inline json create_message(const std::string& role, const std::string& content,
                           const std::string& name = "") {
    json msg;
    msg["role"] = role;
    msg["content"] = content;
    if (!name.empty()) {
        msg["name"] = name;
    }
    return msg;
}

// 辅助函数：创建对话消息列表
inline json create_messages(std::initializer_list<json> messages) {
    json messages_array = json::array();
    for (const auto& msg : messages) {
        messages_array.push_back(msg);
    }
    return messages_array;
}

// 辅助函数：格式化输出（用于调试）
inline std::string format_output(const std::string& output) {
    std::string formatted = output;
    // 替换特殊字符为可见形式
    size_t pos = 0;
    while ((pos = formatted.find('\n', pos)) != std::string::npos) {
        formatted.replace(pos, 1, "\\n\n");
        pos += 3;
    }
    return formatted;
}

// 辅助函数：比较两个字符串（忽略尾部空白）
inline bool compare_outputs(const std::string& actual, const std::string& expected) {
    // 去除尾部空白
    auto rtrim = [](std::string s) {
        s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
        return s;
    };

    return rtrim(actual) == rtrim(expected);
}

// 测试统计类
class TestStats {
private:
    int total_tests = 0;
    int passed_tests = 0;
    std::vector<std::string> failed_test_names;

public:
    void add_result(const TestResult& result) {
        total_tests++;
        if (result.passed) {
            passed_tests++;
        } else {
            failed_test_names.push_back(result.test_name);
        }
    }

    void print_summary() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "测试总结" << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        if (passed_tests == total_tests) {
            std::cout << COLOR_GREEN;
        } else {
            std::cout << COLOR_YELLOW;
        }

        std::cout << "通过: " << passed_tests << "/" << total_tests
                  << " (" << (total_tests > 0 ? passed_tests * 100 / total_tests : 0)
                  << "%)" << COLOR_RESET << std::endl;

        if (!failed_test_names.empty()) {
            std::cout << COLOR_RED << "\n失败的测试:" << COLOR_RESET << std::endl;
            for (const auto& name : failed_test_names) {
                std::cout << "  - " << name << std::endl;
            }
        }

        std::cout << std::string(50, '=') << std::endl;
    }

    bool all_passed() const {
        return passed_tests == total_tests;
    }
};

#endif // TEST_HELPER_H
