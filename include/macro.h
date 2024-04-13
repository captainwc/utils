#ifndef SK_UTILS_MACRO_H
#define SK_UTILS_MACRO_H

// ERRNO
#define EXIT_ASSERT_FAIL    900
#define EXIT_PARAM_ILLEGAL  901

#define THREAD_SAFE_EXIT(x) exit(x)

/// MARK: COMMON

#define ANSI_CLEAR     "\033[0m"
#define ANSI_RED_BG    "\033[0;31m"
#define ANSI_GREEN_BG  "\033[0;32m"
#define ANSI_YELLOW_BG "\033[0;33m"
#define ANSI_BLUE_BG   "\033[0;34m"
#define ANSI_PURPLE_BG "\033[0;35m"

#define COUT(x) std::cout << ANSI_BLUE_BG << "[" #x "]: " << ANSI_CLEAR << sk::printer::toString(x) << std::endl

#define COUT_POSITION "[" << __FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << "]"

#define TODO(msg)                                                                                         \
    do {                                                                                                  \
        std::cerr << ANSI_YELLOW_BG << msg << ANSI_PURPLE_BG << COUT_POSITION << ANSI_CLEAR << std::endl; \
    } while (0);

#define FILL_ME()                 \
    TODO("Fill Code Here!!! -> ") \
    }

#define LINE_BREAKER(msg) \
    std::cout << ANSI_YELLOW_BG << "========== " << (msg) << " ==========" << ANSI_CLEAR << std::endl

#define NEW_LINE() std::cout << std::endl

/// MARK: test

#include <atomic>
#include <cassert>

std::atomic<int> gFailedTest(0);  // NOLINT
std::atomic<int> gTotalTest(0);   // NOLINT

#define ASSERT_MSG(expr, msg)                                             \
    do {                                                                  \
        if (!(expr)) {                                                    \
            std::cerr << ANSI_RED_BG << (msg) << ANSI_CLEAR << std::endl; \
            THREAD_SAFE_EXIT(EXIT_ASSERT_FAIL);                           \
        }                                                                 \
    } while (0)

#define ASSERT(expr)                                                                                      \
    do {                                                                                                  \
        if (!(expr)) {                                                                                    \
            std::cerr << ANSI_RED_BG << "Assert " << ANSI_PURPLE_BG << #expr << ANSI_RED_BG << " Failed!" \
                      << ANSI_CLEAR << std::endl;                                                         \
            THREAD_SAFE_EXIT(EXIT_ASSERT_FAIL);                                                           \
        }                                                                                                 \
    } while (0)

/// x: expected, y: acutal
#define STR_EQUAL_(x, y)                                                                                              \
    do {                                                                                                              \
        auto expected = sk::printer::toString(x);                                                                     \
        auto actual   = sk::printer::toString(y);                                                                     \
        if (expected == actual) {                                                                                     \
            std::cout << ANSI_GREEN_BG << "[PASSED] " << ANSI_CLEAR << "[" + std::to_string(gTotalTest.load()) + "] " \
                      << ANSI_BLUE_BG << #x "==" #y << ANSI_CLEAR << std::endl;                                       \
        } else {                                                                                                      \
            ++gFailedTest;                                                                                            \
            std::cerr << ANSI_RED_BG << "[FAILED] " << ANSI_CLEAR << "[" + std::to_string(gTotalTest.load()) + "] "   \
                      << ANSI_BLUE_BG << #x "==" #y ": " << ANSI_YELLOW_BG << COUT_POSITION << ANSI_CLEAR             \
                      << std::endl;                                                                                   \
            std::cerr << ANSI_PURPLE_BG << '\t' << "Expected: " << ANSI_CLEAR << expected << std::endl;               \
            std::cerr << ANSI_PURPLE_BG << '\t' << "  Actual: " << ANSI_CLEAR << actual << std::endl;                 \
        }                                                                                                             \
        ++gTotalTest;                                                                                                 \
    } while (0)

//? 这里包装只是为了加上 NOLINT 而已。因为没懂为什么clang-tidy会告警：Do not implicitly decay an array into a pointer
#define ASSERT_STR_EQUAL(x, y) STR_EQUAL_(x, y)  // NOLINT

#define ASSERT_ALL_PASSED()                                                                           \
    do {                                                                                              \
        std::cout << std::endl;                                                                       \
        if (gFailedTest.load() == 0) {                                                                \
            std::cout << ANSI_GREEN_BG << "==== "                                                     \
                      << std::to_string(gTotalTest.load()) + "/" + std::to_string(gTotalTest.load())  \
                             + " PASSED ALL! ===="                                                    \
                      << ANSI_CLEAR << std::endl;                                                     \
        } else {                                                                                      \
            std::cout << ANSI_RED_BG << "==== "                                                       \
                      << std::to_string(gFailedTest.load()) + "/" + std::to_string(gTotalTest.load()) \
                             + " test failed! ===="                                                   \
                      << ANSI_CLEAR << std::endl;                                                     \
        }                                                                                             \
    } while (0)

#endif  // SK_UTILS_MACRO_H
