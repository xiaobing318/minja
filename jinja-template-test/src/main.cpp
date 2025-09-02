// main.cpp - 主程序入口
#include <iostream>
#include <string>
#include <memory>
#include <minja/minja.hpp>  // minja C++模板引擎头文件
#include "test_helper.h"

// 声明测试函数（在test_cases.cpp中实现）
void run_basic_tests(TestStats& stats);
void run_message_tests(TestStats& stats);
void run_system_message_tests(TestStats& stats);
void run_tool_message_tests(TestStats& stats);
void run_generation_prompt_tests(TestStats& stats);
void run_edge_case_tests(TestStats& stats);

int main(int argc, char* argv[]) {
    std::cout << COLOR_BLUE << std::string(60, '=') << COLOR_RESET << std::endl;
    std::cout << COLOR_BLUE << "     Minja/Jinja 模板测试程序" << COLOR_RESET << std::endl;
    std::cout << COLOR_BLUE << "     测试模板: openai-gpt-oss-120b.jinja" << COLOR_RESET << std::endl;
    std::cout << COLOR_BLUE << std::string(60, '=') << COLOR_RESET << std::endl;
    std::cout << std::endl;

    // 初始化测试统计
    TestStats stats;

    try {
        // 测试minja库是否正常工作
        std::cout << COLOR_YELLOW << "检查minja库状态..." << COLOR_RESET << std::endl;

        // 创建一个简单的模板测试minja是否正常
        minja::Environment env;
        auto simple_template = env.parse("Hello {{ name }}!");
        json simple_data;
        simple_data["name"] = "Minja";
        std::string result = env.render(simple_template, simple_data);

        if (result == "Hello Minja!") {
            std::cout << COLOR_GREEN << "✓ Minja库工作正常" << COLOR_RESET << std::endl;
        } else {
            std::cout << COLOR_RED << "✗ Minja库测试失败" << COLOR_RESET << std::endl;
            return 1;
        }

        std::cout << std::endl;

        // 运行各类测试
        std::cout << COLOR_BLUE << "=== 基础功能测试 ===" << COLOR_RESET << std::endl;
        run_basic_tests(stats);

        std::cout << "\n" << COLOR_BLUE << "=== 消息处理测试 ===" << COLOR_RESET << std::endl;
        run_message_tests(stats);

        std::cout << "\n" << COLOR_BLUE << "=== 系统消息测试 ===" << COLOR_RESET << std::endl;
        run_system_message_tests(stats);

        std::cout << "\n" << COLOR_BLUE << "=== 工具消息测试 ===" << COLOR_RESET << std::endl;
        run_tool_message_tests(stats);

        std::cout << "\n" << COLOR_BLUE << "=== 生成提示测试 ===" << COLOR_RESET << std::endl;
        run_generation_prompt_tests(stats);

        std::cout << "\n" << COLOR_BLUE << "=== 边界情况测试 ===" << COLOR_RESET << std::endl;
        run_edge_case_tests(stats);

    } catch (const std::exception& e) {
        std::cerr << COLOR_RED << "错误: " << e.what() << COLOR_RESET << std::endl;
        return 1;
    }

    // 打印测试总结
    stats.print_summary();

    // 返回值：全部通过返回0，否则返回1
    return stats.all_passed() ? 0 : 1;
}
