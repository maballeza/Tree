//#pragma once
//#include "../Tree/Tree.hpp"
//
//// Memory Leak Watch
//class TreeTestListener : public ::testing::EmptyTestEventListener {
//public:
//    void OnTestStart(const ::testing::TestInfo& /*testinfo*/) override {
//        allocations = 0;
//    }
//    void OnTestEnd(const ::testing::TestInfo& /*testinfo*/) override {
//        ASSERT_EQ(allocations, 0);
//    }
//
//    static int allocations;
//};
//
//int TreeTestListener::allocations{};
//
//// Client
//template<typename T>
//class TreeTest : public testing::Test {
//protected:
//    void SetUp() override {
//        for (auto Ba : BaKeys) {
//            Insert(tree, Ba, static_cast<T&&>(Ba));
//        }
//    }
//
//    void TearDown() override {
//        for (auto Ba : BaKeys) {
//            Delete(tree, Ba);
//        }
//    }
//
//    T* Insert(Tree<T>& t, const int k, T&& v) {
//        auto success = t.Insert(k, std::forward<T>(v));
//        if (success) {
//            TreeTestListener::allocations++;
//        }
//        return success;
//    }
//    void Delete(Tree<T>& t, const int k) {
//        auto success = t.Delete(k);
//        if (success != T{ -1 }) {
//            TreeTestListener::allocations--;
//        }
//    }
//
//    Tree<T> tree;
//};
//
//// ...