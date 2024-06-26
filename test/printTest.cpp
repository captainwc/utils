#include <list>
#include <map>
#include <string_view>
#include <vector>

#include "logger.h"
#include "macro.h"
#include "printer.h"

using namespace sk::printer;  // NOLINT
using namespace sk::logger;   // NOLINT

struct Person {
    int              age;   // NOLINT
    char             sex;   // NOLINT
    std::string_view name;  // NOLINT

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "[" << name << ", " << age << ", " << (sex == 'm' ? "male" : "female") << "]";
        return ss.str();
    }
};

int main() {
    std::vector<std::vector<int>>   vc{{1, 2}, {3, 4}};
    std::map<int, std::vector<int>> mp{{1, {1, 2}}, {2, {2, 3}}};
    std::list<int>                  lst{1, 2, 3, 4, 5};

    LINE_BREAKER("test utils test");
    ASSERT_STR_EQUAL("[[1, 2], [3, 4]]", toString(vc));
    ASSERT_STR_EQUAL("[{1, [1, 2]}, {2 [2, 3]}]", toString(mp));
    ASSERT_STR_EQUAL("[1, 2, 3, 4, 5]", toString(lst));
    ASSERT_ALL_PASSED();

    LINE_BREAKER("printer test");
    COUT(toString(vc));
    print(mp);
    Person person{18, 'm', "shuaikai"};
    COUT(toString(person));
    dump(toString(vc), toString(mp));

    LINE_BREAKER("logger test");
    LOG("hello {} , I am {}!", "shuaikai", 3.14);
    LOG("hello {} , I am {}!", "shuaikai", 3.14, 6.28);
}
