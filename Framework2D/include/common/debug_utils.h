#pragma once
#include <iostream>
#include <string_view>
#include <unordered_map>
#define NOMINMAX

namespace DebugUtils
{
class Debugger
{
   public:
    static int level;

    // 新增：解析命令行参数
    static void parse_command_line(int argc, char* argv[])
    {
        level = 1;  // 默认级别

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "-d" || arg == "--debug")
            {
                if (i + 1 < argc)
                {
                    level = std::atoi(argv[i + 1]);
                }
            }
        }
    }

    template<typename... Args>
    static void log(int required_level, Args&&... args)
    {
        if (level >= required_level)
        {
            (std::cout << ... << args) << '\n';
        }
    }
};
}  // namespace DebugUtils