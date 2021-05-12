#include "gtest/gtest.h"
#include "TreeTest.hpp"
#include "TreeTestListener.hpp"

REGISTER_TYPED_TEST_SUITE_P(TreeTest,
    Search,
    Minimum,
    Maximum,
    Predecessor,
    Successor,
    Insert,
    Delete); 

using types = ::testing::Types<int, long, float, double>;

INSTANTIATE_TYPED_TEST_SUITE_P(Client, TreeTest, types);

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}