#pragma once
#include "debug_utils.h"

#ifdef NDEBUG
#define AA_DEBUG_ASSERT(expression)
#define AA_DEBUG_ASSERT_EQUAL(expression, value)
#define AA_DEBUG_POST_ASSERT(expression)              void(expression)
#define AA_DEBUG_POST_ASSERT_EQUAL(expression, value) void(expression)
#else
#define AA_DEBUG_ASSERT(expression) \
    aa::frm::Assert(#expression, expression, __FILE__, __LINE__);
#define AA_DEBUG_ASSERT_EQUAL(expression, value) \
    aa::frm::AssertEqual(                        \
        #expression, expression, #value, value, __FILE__, __LINE__)
#define AA_DEBUG_POST_ASSERT(expression) \
    aa::frm::Assert(#expression, expression, __FILE__, __LINE__);
#define AA_DEBUG_POST_ASSERT_EQUAL(expression, value) \
    aa::frm::AssertEqual(                             \
        #expression, expression, #value, value, __FILE__, __LINE__);
#endif
