#include "gtest/gtest.h"
#include "TreeTest.hpp"
#include "TreeTestListener.hpp"

std::vector<std::string> names{
    "char",
    "int",
    "long",
    "float",
    "double"
};
using types = testing::Types<char, int, long, float, double>;

REGISTER_TYPED_TEST_SUITE_P(TreeTest,
    Search,
    Minimum,
    Maximum,
    Predecessor,
    Successor,
    Insert,
    Delete);

template<typename T>
struct TypeName {
    std::string name = typeid(T).name();
};

class TypeNames {
public:
    template<typename T>
    static std::string GetName(int type) {
        TypeName<T> tn;
        std::string s{ "Type" };
        for (auto& n : names) {
            if (tn.name == n) {
                s = n;
            }
        }
        return s;
    }
};

INSTANTIATE_TYPED_TEST_SUITE_P(Param, TreeTest, types, TypeNames);

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}