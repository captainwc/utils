#ifndef SK_UTILS_FUNCTIONAL_STYLE_UTILS_H
#define SK_UTILS_FUNCTIONAL_STYLE_UTILS_H

#include <concepts>
#include <type_traits>

namespace sk::function {

template <typename Fn, typename... Args>
concept Predicate = std::predicate<Fn, Args...>;

template <typename Fn, typename Arg>
concept Consumer = std::invocable<Fn, Arg> && std::same_as<void, std::invoke_result_t<Fn, Arg>>;

// TODO: 是否需要decay呢？
template <typename Fn, typename Rt, typename Arg>
concept Function = std::invocable<Fn, Arg> && std::same_as<Arg, std::invoke_result_t<Fn, Arg>>;

}  // namespace sk::function

#endif  // SK_UTILS_FUNCTIONAL_STYLE_UTILS_H