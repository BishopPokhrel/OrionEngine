#pragma once

#include "OEExceptions.h"
#include <cstdlib>
#include <iostream>
#include <string>

#ifdef OE_DEBUG

#ifdef _MSC_VER
#define OE_DEBUGBREAK() __debugbreak()
#else
#include <signal.h>
#define OE_DEBUGBREAK() raise(SIGTRAP)
#endif

// Core internal assert implementation
#define OE_INTERNAL_ASSERT(type, condition, ...)                                \
    do {                                                                         \
        if (!(condition))                                                       \
        {                                                                       \
            std::string msg = "Assertion failed";                               \
            if constexpr (sizeof...(__VA_ARGS__) > 0)                           \
            {                                                                   \
                msg = Utils::BuildMessage(__VA_ARGS__);                         \
            }                                                                   \
                                                                                \
            std::cerr << "[" << type << "] ASSERTION FAILED: " << msg << "\n"   \
                      << "FILE: " << __FILE__ << "\n"                           \
                      << "LINE: " << __LINE__ << "\n"                           \
                      << "FUNC: " << __FUNCTION__ << std::endl;                 \
                                                                                \
            OE_DEBUGBREAK();                                                    \
            throw OrionEngine::OEBaseException(msg);                            \
        }                                                                       \
    } while (false)

#define OE_CORE_ASSERT(condition, ...) \
    OE_INTERNAL_ASSERT("CORE", (condition), __VA_ARGS__)

#define OE_ASSERT(condition, ...) \
    OE_INTERNAL_ASSERT("CLIENT", (condition), __VA_ARGS__)

#else

#define OE_ASSERT(condition, ...)
#define OE_CORE_ASSERT(condition, ...)

#endif