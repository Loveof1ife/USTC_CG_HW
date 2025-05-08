#include <stdexcept>

#include "common/debug_utils.h"
#include "minidraw_window.h"

int DebugUtils::Debugger::level = 1;  // 静态成员初始化

int main(int argc, char* argv[])
{
    // 新增：解析命令行参数
    DebugUtils::Debugger::parse_command_line(argc, argv);

    try
    {
        USTC_CG::MiniDraw w("MiniDraw");
        if (!w.init())
            return 1;

        w.run();
        return 0;
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }
}