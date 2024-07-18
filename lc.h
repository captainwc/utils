#ifndef SHUAIKAI_LC_H
#define SHUAIKAI_LC_H
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <span>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// ERRNO
#define EXIT_ASSERT_FAIL 900
#define EXIT_PARAM_ILLEGAL 901
#define EXIT_CTRL_C 902

#define THREAD_SAFE_EXIT(x) exit(x)

/// MARK: common MCRO

#define ANSI_CLEAR "\033[0m"
#define ANSI_RED_BG "\033[0;31m"
#define ANSI_GREEN_BG "\033[0;32m"
#define ANSI_YELLOW_BG "\033[0;33m"
#define ANSI_BLUE_BG "\033[0;34m"
#define ANSI_PURPLE_BG "\033[0;35m"

#define COUT(x)                                                            \
    std::cout << ANSI_BLUE_BG << "[" #x "]: " << ANSI_CLEAR << toString(x) \
              << std::endl

#define COUT_POSITION \
    "[" << __FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << "]"

#define TODO(msg)                                                             \
    do {                                                                      \
        std::cerr << ANSI_YELLOW_BG << msg << ANSI_PURPLE_BG << COUT_POSITION \
                  << ANSI_CLEAR << std::endl;                                 \
    } while (0);

#define FILL_ME() TODO("Fill Code Here!!! -> ")

#define LINE_BREAKER(msg)                                 \
    std::cout << ANSI_YELLOW_BG << "========== " << (msg) \
              << " ==========" << ANSI_CLEAR << std::endl

#define NEW_LINE() std::cout << std::endl

/// MARK: test MCRO

std::atomic<int> gFailedTest(0);  // NOLINT
std::atomic<int> gTotalTest(0);   // NOLINT

#define ASSERT_MSG(expr, msg)                                             \
    do {                                                                  \
        if (!(expr)) {                                                    \
            std::cerr << ANSI_RED_BG << (msg) << ANSI_CLEAR << std::endl; \
            THREAD_SAFE_EXIT(EXIT_ASSERT_FAIL);                           \
        }                                                                 \
    } while (0)

#define ASSERT(expr)                                                           \
    do {                                                                       \
        if (!(expr)) {                                                         \
            std::cerr << ANSI_RED_BG << "Assert " << ANSI_PURPLE_BG << #expr   \
                      << ANSI_RED_BG << " Failed!" << ANSI_CLEAR << std::endl; \
            THREAD_SAFE_EXIT(EXIT_ASSERT_FAIL);                                \
        }                                                                      \
    } while (0)

#define ASSERT_TRUE(expr)                                                     \
    do {                                                                      \
        if (expr) {                                                           \
            std::cout << ANSI_GREEN_BG << "[PASSED] " << ANSI_CLEAR           \
                      << "[" + std::to_string(gTotalTest.load()) + "] "       \
                      << ANSI_BLUE_BG << #expr << ANSI_GREEN_BG << " => true" \
                      << ANSI_CLEAR << std::endl;                             \
        } else {                                                              \
            ++gFailedTest;                                                    \
            std::cerr << ANSI_RED_BG << "[FAILED] " << ANSI_CLEAR             \
                      << "[" + std::to_string(gTotalTest.load()) + "] "       \
                      << ANSI_BLUE_BG << #expr << ANSI_RED_BG                 \
                      << " => istrue? " << ANSI_YELLOW_BG << COUT_POSITION    \
                      << ANSI_CLEAR << std::endl;                             \
            std::cerr << ANSI_PURPLE_BG << '\t' << "Expected: " << ANSI_CLEAR \
                      << "True" << std::endl;                                 \
            std::cerr << ANSI_PURPLE_BG << '\t' << "  Actual: " << ANSI_CLEAR \
                      << "False" << std::endl;                                \
        }                                                                     \
        ++gTotalTest;                                                         \
    } while (0)

/// x: expected, y: acutal
#define ASSERT_EQUAL(x, y)                                                    \
    do {                                                                      \
        if (x == y) {                                                         \
            std::cout << ANSI_GREEN_BG << "[PASSED] " << ANSI_CLEAR           \
                      << "[" + std::to_string(gTotalTest.load()) + "] "       \
                      << ANSI_BLUE_BG << #x << ANSI_GREEN_BG                  \
                      << " == " << ANSI_BLUE_BG << #y << ANSI_CLEAR           \
                      << std::endl;                                           \
        } else {                                                              \
            ++gFailedTest;                                                    \
            std::cerr << ANSI_RED_BG << "[FAILED] " << ANSI_CLEAR             \
                      << "[" + std::to_string(gTotalTest.load()) + "] "       \
                      << ANSI_BLUE_BG << #x                                   \
                      << ANSI_RED_BG " == " << ANSI_BLUE_BG << #y " "         \
                      << ANSI_YELLOW_BG << COUT_POSITION << ANSI_CLEAR        \
                      << std::endl;                                           \
            std::cerr << ANSI_PURPLE_BG << '\t' << "Expected: " << ANSI_CLEAR \
                      << "Equal" << std::endl;                                \
            std::cerr << ANSI_PURPLE_BG << '\t' << "  Actual: " << ANSI_CLEAR \
                      << "NonEqual" << std::endl;                             \
        }                                                                     \
        ++gTotalTest;                                                         \
    } while (0)

/// x: expected, y: acutal
#define STR_EQUAL_(x, y)                                                      \
    do {                                                                      \
        auto expected = toString(x);                                          \
        auto actual = toString(y);                                            \
        if (expected == actual) {                                             \
            std::cout << ANSI_GREEN_BG << "[PASSED] " << ANSI_CLEAR           \
                      << "[" + std::to_string(gTotalTest.load()) + "] "       \
                      << ANSI_BLUE_BG << #x << ANSI_GREEN_BG                  \
                      << " == " << ANSI_BLUE_BG << #y << ANSI_CLEAR           \
                      << std::endl;                                           \
        } else {                                                              \
            ++gFailedTest;                                                    \
            std::cerr << ANSI_RED_BG << "[FAILED] " << ANSI_CLEAR             \
                      << "[" + std::to_string(gTotalTest.load()) + "] "       \
                      << ANSI_BLUE_BG << #x << ANSI_RED_BG                    \
                      << " == " << ANSI_BLUE_BG << #y " " << ANSI_YELLOW_BG   \
                      << COUT_POSITION << ANSI_CLEAR << std::endl;            \
            std::cerr << ANSI_PURPLE_BG << '\t' << "Expected: " << ANSI_CLEAR \
                      << expected << std::endl;                               \
            std::cerr << ANSI_PURPLE_BG << '\t' << "  Actual: " << ANSI_CLEAR \
                      << actual << std::endl;                                 \
        }                                                                     \
        ++gTotalTest;                                                         \
    } while (0)

#define ASSERT_STR_EQUAL(x, y) STR_EQUAL_(x, y)

#define ASSERT_ALL_PASSED()                                         \
    do {                                                            \
        std::cout << std::endl;                                     \
        if (gFailedTest.load() == 0) {                              \
            std::cout << ANSI_GREEN_BG << "==== "                   \
                      << std::to_string(gTotalTest.load()) + "/" +  \
                             std::to_string(gTotalTest.load()) +    \
                             " PASSED ALL! ===="                    \
                      << ANSI_CLEAR << std::endl;                   \
        } else {                                                    \
            std::cout << ANSI_RED_BG << "==== "                     \
                      << std::to_string(gFailedTest.load()) + "/" + \
                             std::to_string(gTotalTest.load()) +    \
                             " test failed! ===="                   \
                      << ANSI_CLEAR << std::endl;                   \
        }                                                           \
    } while (0)

// 让X语句循环n次
#define LOOP_N(n, X)        \
    do {                    \
        int cnt = n;        \
        while (cnt-- > 0) { \
            X;              \
        }                   \
    } while (0);

// 统计 X 执行单次耗时
#define TIME(msg, X)                                                           \
    do {                                                                       \
        auto start = std::chrono::steady_clock::now();                         \
        X;                                                                     \
        auto end = std::chrono::steady_clock::now();                           \
        auto miliduring =                                                      \
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start) \
                .count();                                                      \
        std::cout << "[" << msg << "]: " << miliduring << "ms\n";              \
    } while (0);

// 让X执行n次，统计耗时与平均耗时
#define TIME_N(msg, n, X)                                                     \
    do {                                                                      \
        int cnt = n;                                                          \
        auto start = std::chrono::steady_clock::now();                        \
        while (cnt-- > 0) {                                                   \
            X;                                                                \
        }                                                                     \
        auto end = std::chrono::steady_clock::now();                          \
        auto cost = end - start;                                              \
        auto miliduring =                                                     \
            std::chrono::duration_cast<std::chrono::milliseconds>(cost)       \
                .count();                                                     \
        auto microduring =                                                    \
            std::chrono::duration_cast<std::chrono::microseconds>(cost)       \
                .count();                                                     \
        std::cout << "[" << n << " " << msg << "]: " << miliduring << "ms, (" \
                  << microduring / n << "us/per)\n";                          \
    } while (0);

// 测试X执行n次耗时。对msg进行了包装，直接传入X的名称
#define SCALE 1000
#define TEST_N(scale, X) TIME_N(#X, scale, X)
#define TEST(X) TIME_N(#X, SCALE, X)

/// MARK: printer
struct ListNode;
struct TreeNode;

template <typename T>
concept LeetcodePointerType =
    std::is_same_v<ListNode *, T> || std::is_same_v<TreeNode *, T>;

template <typename T>
concept StreamOutable = requires(std::ostream &os, T elem) {
    { os << elem } -> std::same_as<std::ostream &>;
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
concept Printable = StreamOutable<T> || Serializable<T> ||
    SequentialContainer<T> || MappedContainer<T> || PairLike<T>;

template <bool>
auto toString(bool obj);

template <Printable T>
auto toString(const T &obj);

template <typename T>
requires Printable<typename T::value_type>
auto forBasedContainer2String(const T &c);

template <SequentialContainer T>
requires Printable<typename T::value_type>
auto SequentialContainer2String(const T &c);

template <PairLike T>
requires Printable<typename T::first_type> && Printable<typename T::second_type>
auto Pair2String(const T &p);

template <MappedContainer T>
requires Printable<typename T::key_type> && Printable<typename T::mapped_type>
auto MappedContainer2String(const T &c);

/// MARK: LEETCODE

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    explicit ListNode(int v) : val(v), next(nullptr) {}
    ListNode(int v, ListNode *n) : val(v), next(n) {}
    ~ListNode() { delete next; }
    string toString();
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x = 0, TreeNode *left = nullptr, TreeNode *right = nullptr)
        : val(x), left(left), right(right) {}
    ~TreeNode() {
        delete left;
        delete right;
    }
    string toString();
};

/// MARK: tree/List tools
inline string treeToString(TreeNode *root, string sep) {
    string ret;
    // inorder
    stringstream ss;
    TreeNode *p = root;
    ss << "(In)[";
    std::function<void(TreeNode *)> inorder = [&](TreeNode *node) {
        if (node != nullptr) {
            inorder(node->left);
            ss << node->val << sep;
            inorder(node->right);
        }
    };
    inorder(p);
    ret = ss.str().substr(0, ss.str().length() - sep.length()) + "]";
    // preorder
    stringstream ss2;
    ss2 << "(Pre)[";
    std::function<void(TreeNode *)> preorder = [&](TreeNode *node) {
        if (node != nullptr) {
            ss2 << node->val << sep;
            preorder(node->left);
            preorder(node->right);
        }
    };
    TreeNode *q = root;
    preorder(q);
    string ret2 = ss2.str().substr(0, ss2.str().length() - sep.length()) + "]";
    return ret2 + sep + ret;
}
inline string listToString(ListNode *list, string sep) {
    stringstream ss;
    ListNode *p = list;
    ss << "[";
    while (p != nullptr) {
        ss << p->val << sep;
        p = p->next;
    }
    return ss.str().substr(0, ss.str().length() - sep.length()) + "]";
}

string TreeNode::toString() { return treeToString(this, ", "); }
string ListNode::toString() { return listToString(this, ", "); }

TreeNode *buildTree(span<int> preorder, span<int> inorder) {
    TreeNode *root = new TreeNode(preorder[0]);
    int len = preorder.size();
    if (len == 1) {
        return root;
    }
    int idx = -1;
    for (int i = 0; i < len; i++) {
        if (inorder[i] == preorder[0]) {
            idx = i;
        }
    }
    root->left = buildTree(preorder.subspan(1, idx), inorder.subspan(0, idx));
    root->right = buildTree(preorder.subspan(idx + 1, len - idx - 1),
                            inorder.subspan(idx + 1, len - idx - 1));
    return root;
}

inline ListNode *buildList(vector<int> &&data) {
    int len = data.size();
    auto head = new ListNode(data[0]);
    auto tail = head;
    for (int i = 1; i < len; i++) {
        tail->next = new ListNode(data[i]);
        tail = tail->next;
    }
    return head;
}

inline ListNode *buildList(vector<int> &data) {
    return buildList(std::move(data));
}

inline void reverseList(ListNode **head) {
    ListNode *p = *head;
    ListNode *q = p->next;
    if (q == nullptr) {
        return;
    }
    p->next = nullptr;
    while (q->next != nullptr) {
        ListNode *r = q->next;
        q->next = p;
        p = q;
        q = r;
    }
    q->next = p;
    *head = q;
}

/// MARK: printer Impl

template <PairLike T>
requires Printable<typename T::first_type> && Printable<typename T::second_type>
auto Pair2String(const T &p) {
    std::stringstream ss;
    ss << '{' << toString(std::get<0>(p)) << ", " << toString(std::get<1>(p))
       << '}';
    return ss.str();
}

template <typename T>
requires Printable<typename T::value_type>
auto forBasedContainer2String(const T &c) {
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
}

template <SequentialContainer T>
requires Printable<typename T::value_type>
auto SequentialContainer2String(const T &c) {
    return forBasedContainer2String(c);
}

template <MappedContainer T>
requires Printable<typename T::key_type> && Printable<typename T::mapped_type>
auto MappedContainer2String(const T &c) { return forBasedContainer2String(c); }

template <Printable T>
auto toString(const T &obj) {
    // 优先使用自带的 toString() 方法
    if constexpr (LeetcodePointerType<T>) {
        return obj->toString();
    } else if constexpr (Serializable<T>) {
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
void print(const T &obj, const std::string &prefix = "",
           const std::string &suffix = "", bool lineBreak = true) {
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

/// MARK: log

#define LOG(...) LOG_(__VA_ARGS__)  // NOLINT
#define LOG_(...)                                                         \
    do {                                                                  \
        std::cout << ANSI_BLUE_BG << COUT_POSITION << ": " << ANSI_CLEAR; \
        println(__VA_ARGS__);                                             \
    } while (0)

int constexpr countSubStr(std::string_view str, std::string_view substr) {
    int cnt = 0;
    size_t pos = 0;
    auto sublength = substr.length();
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        cnt++;
        pos += sublength;
    }
    return cnt;
}

template <typename... Args>
std::string formatStr(std::string_view format, const Args &...args) {
    int placeholdersCount = countSubStr(format, "{}");
    ASSERT_MSG((placeholdersCount == sizeof...(args)),
               "Failed to call LOG, because num of {} must equal to args you "
               "passed! " +
                   formatStr("Expected {} args, and got {}.", placeholdersCount,
                             sizeof...(args)));
    std::string logMsg(format);
    size_t argIndex = 0;
    ((logMsg.replace(logMsg.find("{}"), 2, toString(args)), ++argIndex), ...);
    return logMsg;
}

template <typename... Args>
void println(std::string_view format, const Args &...args) {
    std::cout << formatStr(format, args...) << std::endl;
}

/// MARK: random util

inline int getRandomInt(int start = 0, int end = 100) {
    auto seed = random_device{"/dev/random"}();
    auto engine = mt19937{seed};
    auto distro = uniform_int_distribution<int>(start, end);
    return distro(engine);
    // auto distro =
    //     normal_distribution<double>((start + end) / 2, (end - start) / 6);
    // return (int)(distro(engine));
}

inline double getRandomDouble(double start = 0, double end = 1) {
    auto engine = mt19937{random_device{}()};
    auto distro = normal_distribution<double>(start, end);
    return distro(engine);
}

inline vector<int> getRandomIntVector(int len = 10, int start = 0,
                                      int end = 100, bool uniqueElem = true) {
    vector<int> vc;
    unordered_set<int> elemset;
    vc.reserve(len);
    if (uniqueElem) {
        if ((end - start + 1) < len) {
            throw std::runtime_error(
                "Can't satisfy uniqueElem cause range is even smaller than "
                "length");
        }
        while (len > 0) {
            int elem = getRandomInt(start, end);
            if (elemset.find(elem) == elemset.end()) {
                elemset.emplace(elem);
                vc.push_back(elem);
                len--;
            } else {
                continue;
            }
        }
    } else {
        while (len-- > 0) {
            int elem = getRandomInt(start, end);
            vc.push_back(elem);
        }
    }
    return vc;
}

inline string getRandomString(int len = 10, bool uniqueElem = false) {
    string charset1 = R"(
        !#$%&'()*+,-./"
        0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`"
        abcdefghijklmnopqrstuvwxyz{|}~)";
    string charset2 =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string charset3 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
#define CHARSET charset3
    stringstream ss;
    if (uniqueElem) {
        if (CHARSET.size() < len) {
            throw std::runtime_error(
                "Can't satisfy uniqueElem cause range is even smaller than "
                "length");
        }
        unordered_set<int> idxSet;
        while (len > 0) {
            int idx = getRandomInt(0, CHARSET.size() - 1);
            if (idxSet.find(idx) == idxSet.end()) {
                ss << CHARSET[idx];
                len--;
            } else {
                continue;
            }
        }
    } else {
        while (len-- > 0) {
            ss << CHARSET[getRandomInt(0, CHARSET.size() - 1)];
        }
    }
    return ss.str();
}

/// MARK: ppm img

struct color {
    int r;
    int g;
    int b;
};

namespace Colors {
const color BLACK{0, 0, 0};
const color WHITE{255, 255, 255};
const color RED{255, 0, 0};
const color GREEN{0, 255, 0};
const color BLUE{0, 0, 255};
color map_int_to_color(int x) {
    if (x < 20) {
        return WHITE;
    } else if (x < 40) {
        return GREEN;
    } else if (x < 60) {
        return BLUE;
    } else if (x < 80) {
        return RED;
    } else {
        return BLACK;
    }
}
}  // namespace Colors
using namespace Colors;

class Image {
   public:
    explicit Image(string &&filename = "lc_ppm_demo", int width = 100,
                   int height = 100, string &&folder = "pic")
        : img_(height, vector<color>(width, GREEN)),
          picname_(move(filename)),
          width_(width),
          height_(height),
          save_loc_(get_path(folder)) {
        auto r = picname_.find(".");
        if (r != string::npos) {
            picname_ = picname_.substr(0, r);
        }
    }
    explicit Image(vector<vector<color>> &&data,
                   string &&filename = "lc_ppm_demo", string &&folder = "pic")
        : img_(move(data)),
          picname_(move(filename)),
          width_(img_[0].size()),
          height_(img_.size()),
          save_loc_(get_path(folder)) {}

    Image(const vector<vector<int>> &data, string filename = "lc_ppm_demo",
          std::function<color(int)> mapper = Colors::map_int_to_color,
          string &&folder = "pic")
        : img_(data.size(), vector<color>(data[0].size(), Colors::BLACK)),
          picname_(move(filename)),
          width_(data[0].size()),
          height_(data.size()),
          save_loc_(get_path(folder)) {
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                img_[i][j] = mapper(data[i][j]);
            }
        }
    }
    Image(const vector<int> &data, string filename = "lc_ppm_demo",
          std::function<color(int)> mapper = Colors::map_int_to_color,
          int height = 100, string &&folder = "pic")
        : img_(height, vector<color>(data.size(), Colors::BLACK)),
          picname_(filename),
          width_(data.size()),
          height_(height),
          save_loc_(get_path(folder)) {
        int max = *max_element(data.begin(), data.end());
        if (max < height) {
            for (int j = 0; j < width_; j++) {
                for (int i = height_ - 1; i >= height_ - data[j]; i--) {
                    img_[i][j] = mapper(data[j]);
                }
            }
        } else {
            FILL_ME();
        }
    }

    Image(const Image &another)
        : img_(another.img_),
          picname_(another.picname_),
          width_(another.width_),
          height_(another.height_) {}

    Image(Image &&another) = delete;
    Image &operator=(const Image &another) = delete;
    Image &operator=(Image &&another) = delete;

    vector<color> &operator[](int row) { return img_[row]; }

    void draw(string_view filetype = "jpg") {
        if (!write_to_file()) {
            cerr << "[FAILED] failed to write ppm file\n";
            return;
        }
        if (!convert(filetype)) {
            cerr << "[FAILED] failed to convert ppm file\n";
            return;
        }
        string target_file(picname_);
        target_file.append(".").append(filetype.data());
        if (!save(target_file)) {
            fprintf(stderr, "[FAILED] cannot move %s to dest\n",
                    target_file.c_str());
        }
    }

   private:
    vector<vector<color>> img_;
    string picname_;
    int width_;
    int height_;
    string save_loc_;

    string get_path(string folder) {
        string ret("/mnt/d/test");
        if (folder == "") {
            return ret;
        }
        return ret.append("/").append(folder);
    }

    bool write_to_file() {
        auto ppm_file = picname_ + ".ppm";
        auto fd = fopen(ppm_file.c_str(), "w");
        if (!fd) {
            return false;
        }
        std::atomic_bool write_finish = false;
        std::thread loader_thread([&write_finish]() {
            const char spinner[] = {'\\', '|', '/', '-'};
            int i = 0;
            while (!write_finish.load()) {
                std::cout << "\r" << spinner[i++ % 4] << " " << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            std::cout << "\r" << std::flush;  // Clear the spinner
        });
        fprintf(fd, "P6\n%d %d 255\n", width_, height_);
        for (auto &vc : img_) {
            for (auto &c : vc) {
                fprintf(fd, "%c%c%c", c.r, c.g, c.b);
            }
        }
        fclose(fd);
        write_finish = true;
        loader_thread.join();
        return true;
    }

    bool convert(string_view filetype = "jpg") {
        auto target_file = picname_ + "." + filetype.data();
        auto convert_cmd = "convert " + picname_ + ".ppm " + target_file;
        auto rm_cmd = "rm " + picname_ + ".ppm";

        std::atomic<bool> convert_finished = false;
        std::thread loader_thread([&convert_finished]() {
            const char spinner[] = {'\\', '|', '/', '-'};
            int i = 0;
            while (!convert_finished.load()) {
                std::cout << "\r" << spinner[i++ % 4] << " " << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            std::cout << "\r" << std::flush;  // Clear the spinner
        });

        FILE *convert_pipe = popen(convert_cmd.c_str(), "r");
        if (convert_pipe) {
            pclose(convert_pipe);
            convert_finished = true;
            loader_thread.join();
        } else {
            loader_thread.detach();
            return false;
        }

        FILE *rm_pipe = popen(rm_cmd.c_str(), "r");
        if (rm_pipe) {
            pclose(rm_pipe);
            return true;
        }
        return false;
    }

    bool save(string_view file) {
        if (!std::filesystem::exists(save_loc_)) {
            try {
                std::filesystem::create_directory(save_loc_);
            } catch (const std::filesystem::filesystem_error ex) {
                cerr << ex.what() << '\n';
                return false;
            }
        }
        string mv_cmd("mv ");
        mv_cmd.append(file.data())
            .append(" ")
            .append(save_loc_)
            .append("/")
            .append(file.data());
        auto mv_ret = popen(mv_cmd.c_str(), "r");
        if (mv_ret) {
            pclose(mv_ret);
            return true;
        }
        return false;
    }
};

#endif  // SHUAIKAI_LC_H
