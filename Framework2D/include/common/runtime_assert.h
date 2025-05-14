#pragma once
// #include <boost/stacktrace.hpp
#include <iostream>
#include <string_view>

#include "debug_utils.h"
namespace DebugUtils
{
class Assertion
{
   public:
    static bool Enabled()
    {
        if (!inited_)
            Abort("assertion is not enabled yet!");
        return enabled_;
    };
    static void Init(bool enable)
    {
        inited_ = true;
        enabled_ = enable;
    };

   private:
    static bool inited_;
    static bool enabled_;
};
}  // namespace DebugUtils

#define AA_RUNTIME_ASSERT(expression)     \
    if (DebugUtils::Assertion::Enabled()) \
        DebugUtils::Assert(#expression, expression, __FILE__, __LINE__);

#define AA_RUNTIME_ASSERT_EQUAL(expression, value) \
    if (DebugUtils::Assertion::Enabled())          \
    DebugUtils::AssertEqual(                       \
        #expression, expression, #value, value, __FILE__, __LINE__)

#define AA_RUNTIME_POST_ASSERT(expression)                               \
    if (DebugUtils::Assertion::Enabled())                                \
    {                                                                    \
        DebugUtils::Assert(#expression, expression, __FILE__, __LINE__); \
    }                                                                    \
    else                                                                 \
    {                                                                    \
        void(expression);                                                \
    }

#define AA_RUNTIME_POST_ASSERT_EQUAL(expression, value)                  \
    if (DebugUtils::Assertion::Enabled())                                \
    {                                                                    \
        DebugUtils::AssertEqual(                                         \
            #expression, expression, #value, value, __FILE__, __LINE__); \
    }                                                                    \
    else                                                                 \
    {                                                                    \
        void(expression);                                                \
    }
