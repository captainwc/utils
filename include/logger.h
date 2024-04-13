#ifndef SK_UTILS_LOGGER_H
#define SK_UTILS_LOGGER_H

#include <iostream>

#include "macro.h"
#include "printer.h"

#define LOG(...) LOG_(__VA_ARGS__)  // NOLINT
#define LOG_(...)                                                         \
    do {                                                                  \
        std::cout << ANSI_BLUE_BG << COUT_POSITION << ": " << ANSI_CLEAR; \
        sk::logger::log(__VA_ARGS__);                                     \
    } while (0)

namespace sk::logger {
int constexpr countSubStr(std::string_view str, std::string_view substr) {
    int    cnt       = 0;
    size_t pos       = 0;
    auto   sublength = substr.length();
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        cnt++;
        pos += sublength;
    }
    return cnt;
}

template <typename... Args>
std::string formatStr(std::string_view format, const Args&... args) {
    int placeholdersCount = countSubStr(format, "{}");
    ASSERT_MSG((placeholdersCount == sizeof...(args)),
               "Failed to call LOG, because num of {} must equal to args you passed! "
                   + formatStr("Expected {} args, and got {}.", placeholdersCount, sizeof...(args)));
    std::string logMsg(format);
    size_t      argIndex = 0;
    ((logMsg.replace(logMsg.find("{}"), 2, sk::printer::toString(args)), ++argIndex), ...);
    return logMsg;
}

template <typename... Args>
void log(std::string_view format, const Args&... args) {
    std::cout << formatStr(format, args...) << std::endl;
}
}  // namespace sk::logger
#endif  // SK_UTILS_LOGGER_H