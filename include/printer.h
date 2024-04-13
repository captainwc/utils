#ifndef SK_UTILS_PRINTER_H
#define SK_UTILS_PRINTER_H

/**
 * @file printer.h
 * @brief Header Only, 提供简单的输出任意类型、日志、测试宏等功能，可以彩色输出；要求 c++20 及以上
 * @usage:
 *      打印：
 *        - 对于常见类型（基本类型、STL container、重载了 << 操作符的类型，以及他们的嵌套，可以直接 print 和 toString
 *        - 对于自定义类型，需要实现一个 toString 函数（返回值为任意可转化为string_view的类型），或者重载 << 操作符
 *        - print(obj) 直接打印
 *        - dump(...) 连续打印
 *        - OUT(obj) 打印 [#obj]: obj的形式
 *      日志：
 *        - LOG(...)
 *      测试：
 *        - ASSERT_STR_EQUAL(expect, actual) 简单的测试
 *               将两者转换为字符串进行比较，字符串要求同“打印”（注意，由于转为字符串比较，所以 1 == “1”）
 *        - ASSERT_ALL_PASSED() 统计用例通过情况
 *        - ASSERT(expr)
 *        - ASSERT_MSG(expr, msg)
 *      其他：
 *        - LINE_BREAKER(msg) 输出一个黄色的分割线，中间是信息
 *        - TODO() 输出需要补全代码的位置信息
 * @copyright Copyright (c) shuaikai 2024
 */
static_assert(__cplusplus >= 202002L, "To use this file, your cpp version must >= CXX20");

#include <iostream>
#include <sstream>

#include "macro.h"

/// MARK: Concepts

namespace sk::printer {

template <typename T>
concept StreamOutable = requires(std::ostream& os, T elem) {
    { os << elem } -> std::same_as<std::ostream&>;
};
template <typename T>
concept Serializable = requires(T obj) {
    { obj.toString() } -> std::convertible_to<std::string_view>;
};
template <typename T>
concept SequentialContainer = requires(T c) {
    typename T::value_type;
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
};
template <typename T>
concept MappedContainer = requires(T m) {
    typename T::key_type;
    typename T::mapped_type;
    { m.begin() } -> std::same_as<typename T::iterator>;
    { m.end() } -> std::same_as<typename T::iterator>;
};
template <typename T>
concept PairLike = requires(T p) {
    { std::get<0>(p) } -> std::convertible_to<typename T::first_type>;
    { std::get<1>(p) } -> std::convertible_to<typename T::second_type>;
};
template <typename T>
concept Printable = StreamOutable<T> || Serializable<T> || SequentialContainer<T> || MappedContainer<T> || PairLike<T>;

/// MARK: 前向声明

template <bool>
auto toString(bool obj);

template <Printable T>
auto toString(const T& obj);

template <typename T>
    requires Printable<typename T::value_type>
auto forBasedContainer2String(const T& c);

template <SequentialContainer T>
    requires Printable<typename T::value_type>
auto SequentialContainer2String(const T& c);

template <PairLike T>
    requires Printable<typename T::first_type> && Printable<typename T::second_type>
auto Pair2String(const T& p);

template <MappedContainer T>
    requires Printable<typename T::key_type> && Printable<typename T::mapped_type>
auto MappedContainer2String(const T& c);

/// MARK: Implementions

template <PairLike T>
    requires Printable<typename T::first_type> && Printable<typename T::second_type>
auto Pair2String(const T& p) {
    std::stringstream ss;
    ss << '{' << toString(std::get<0>(p)) << ", " << toString(std::get<1>(p)) << '}';
    return ss.str();
}

template <typename T>
    requires Printable<typename T::value_type>
auto forBasedContainer2String(const T& c) {
    std::stringstream ss;
    ss << "[";
    for (auto elem : c) {
        ss << toString(elem) << ", ";
    }
    auto ret = ss.str();
    ret.pop_back();
    ret.pop_back();
    ret.push_back(']');
    return ret;
    // NOTE: 为什么这里用这种方法，打印出来的结果看着正常，但是进行字符串比较的时候会不一致
    // ss.seekp(-2, std::ios_base::end);
    // ss << "]" << '\0';
    // return ss.str();
}

template <SequentialContainer T>
    requires Printable<typename T::value_type>
auto SequentialContainer2String(const T& c) {
    return forBasedContainer2String(c);
}

template <MappedContainer T>
    requires Printable<typename T::key_type> && Printable<typename T::mapped_type>
auto MappedContainer2String(const T& c) {
    return forBasedContainer2String(c);
}

template <Printable T>
auto toString(const T& obj) {
    // 优先使用自带的 toString() 方法
    if constexpr (Serializable<T>) {
        return obj.toString();
    } else if constexpr (StreamOutable<T>) {
        std::stringstream ss;
        ss << obj;
        return std::move(ss.str());
    } else if constexpr (SequentialContainer<T>) {
        return SequentialContainer2String(obj);
    } else if constexpr (MappedContainer<T>) {
        return MappedContainer2String(obj);
    } else if constexpr (PairLike<T>) {
        return Pair2String(obj);
    } else {
        std::cerr << ANSI_RED_BG << "Isn't Printable\n" << ANSI_CLEAR;
        return "@@FALSE_STRING@@";
    }
}

template <bool>
auto toString(bool obj) {
    return obj ? "Ture" : "False";
}

template <Printable T>
void print(const T& obj, const std::string& prefix = "", const std::string& suffix = "", bool lineBreak = true) {
    std::cout << prefix << toString(obj) << suffix;
    if (lineBreak) {
        std::cout << "\n";
    }
}

template <Printable... Args>
void dump(Args... args) {
    ((std::cout << toString(args) << " "), ...);
    std::cout << "\n";
}

}  // namespace sk::printer

#endif  // SK_UTILS_PRINTER_H
