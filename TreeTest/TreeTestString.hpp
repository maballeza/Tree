#pragma once
#include <gtest/gtest.h>
#include <string>
#include "../Tree.hpp"

/**
* class TreeTestString
*   Type parameterized test for the Tree interface.
*/
class TreeTestString : public testing::Test {
protected:
    void SetUp() override {
        short Ba[10] = { 5, 6, 7, 8, 9, 4, 3, 2, 1, 0 };
        short Br[10] = { 5, 4, 2, 3, 1, 0, 6, 9, 7, 8 };
        /**
        * Branching Tree       5
        *                     / \
        *                    4   6
        *                   /     \
        *                  2       9
        *                 / \     /
        *                1   3   7
        *               /         \
        *              0           8
        */

        for (auto& k : keys) {
            BalancedTr.Insert(Ba[k], std::move(std::to_wstring(Ba[k])));
        }
        for (auto& k : keys) {
            BranchingTr.Insert(Br[k], std::move(std::to_wstring(Br[k])));
        }
    }

    Tree<short, std::wstring> EmptyTr;
    Tree<short, std::wstring> BalancedTr;
    Tree<short, std::wstring> BranchingTr;

    const std::vector<short>  keys{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const std::vector<short> rkeys{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
};