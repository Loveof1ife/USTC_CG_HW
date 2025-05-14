#include <stdexcept>

#include "common/debug_utils.h"
#include "common/runtime_assert.h"
#include "poisson_window.h"

int DebugUtils::Debugger::level = 0;
bool DebugUtils::Assertion::inited_ = false;
bool DebugUtils::Assertion::enabled_ = false;

int main()
{
    DebugUtils::Debugger::parse_command_line(__argc, __argv);

    DebugUtils::Assertion::Init(DebugUtils::Debugger::level > 0);

    try
    {
        USTC_CG::PoissonWindow w("Poisson Image Editing");
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