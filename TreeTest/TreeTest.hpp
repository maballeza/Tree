#pragma once
#include "../Tree.hpp"

/**
 * class TreeTest
 *   Type parameterized test for the Tree interface.
 */
template<typename T>
class TreeTest : public testing::Test {
protected:
    void SetUp() override {
        int Ba[10] = { 5, 6, 7, 8, 9, 4, 3, 2, 1, 0 };
        int Br[10] = { 5, 4, 2, 3, 1, 0, 6, 9, 7, 8 };

        for (auto k : keys) {
            Balanced.Insert(Ba[k], static_cast<T&&>(Ba[k]));
        }
        for (auto k : keys) {
            Branching.Insert(Br[k], static_cast<T&&>(Br[k]));
        }
    }

    Tree<T> Empty;
    Tree<T> Balanced;
    Tree<T> Branching;

    const std::vector<int> keys{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const std::vector<int> rkeys{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
};

TYPED_TEST_SUITE_P(TreeTest);

/**
 * Search
 *   Returns the found value if successful and nullptr otherwise.
 */
TYPED_TEST_P(TreeTest, Search) {
    for (auto valid : this->keys) {
        EXPECT_EQ(static_cast<TypeParam>(valid), *this->Balanced.Search(valid));
        EXPECT_EQ(static_cast<TypeParam>(valid), *this->Balanced.Search(valid));
        EXPECT_EQ(static_cast<TypeParam>(valid), *this->Branching.Search(valid));
    }

    EXPECT_EQ(nullptr, this->Balanced.Search(this->keys.size()));
    EXPECT_EQ(nullptr, this->Branching.Search(this->keys.size()));
    EXPECT_EQ(nullptr, this->Empty.Search(this->keys.size()));
}

/**
 * Minimum & Maximum
 *   Returns the found value if successful and nullptr for an empty tree.
 */
TYPED_TEST_P(TreeTest, Minimum) {
    for (auto val : this->keys) {
        EXPECT_EQ(static_cast<TypeParam>(val), *this->Balanced.Minimum());
        EXPECT_EQ(static_cast<TypeParam>(val), *this->Branching.Minimum());
        this->Balanced.Delete(val);
        this->Branching.Delete(val);
    }

    EXPECT_EQ(nullptr, this->Empty.Minimum());
}

TYPED_TEST_P(TreeTest, Maximum) {
    for (auto val : this->rkeys) {
        EXPECT_EQ(static_cast<TypeParam>(val), *this->Balanced.Maximum());
        EXPECT_EQ(static_cast<TypeParam>(val), *this->Branching.Maximum());
        this->Balanced.Delete(val);
        this->Branching.Delete(val);
    }

    EXPECT_EQ(nullptr, this->Empty.Maximum());
}

/**
 * Predecessor & Successor
 *   If the key is valid, returns either the corresponding value or itself; 
 *   otherwise it returns nullptr.
 */
TYPED_TEST_P(TreeTest, Predecessor) {
    for (auto val : this->keys) {
        if (val == this->keys.front()) {
            EXPECT_EQ(static_cast<TypeParam>(val), *this->Balanced.Predecessor(val));
            EXPECT_EQ(static_cast<TypeParam>(val), *this->Branching.Predecessor(val));
        }
        else {
            EXPECT_EQ(static_cast<TypeParam>(val - 1), *this->Balanced.Predecessor(val));
            EXPECT_EQ(static_cast<TypeParam>(val - 1), *this->Branching.Predecessor(val));
        }
    }
    
    EXPECT_EQ(nullptr, this->Empty.Predecessor(this->keys.size()));
    EXPECT_EQ(nullptr, this->Empty.Predecessor(this->keys.size()));
}

TYPED_TEST_P(TreeTest, Successor) {
    for (auto val : this->keys) {
        if (val == this->keys.back()) {
            EXPECT_EQ(static_cast<TypeParam>(val), *this->Balanced.Successor(val));
            EXPECT_EQ(static_cast<TypeParam>(val), *this->Branching.Successor(val));
        }
        else {
            EXPECT_EQ(static_cast<TypeParam>(val + 1), *this->Balanced.Successor(val));
            EXPECT_EQ(static_cast<TypeParam>(val + 1), *this->Branching.Successor(val));
        }
    }
    
    EXPECT_EQ(nullptr, this->Empty.Successor(this->keys.size()));
    EXPECT_EQ(nullptr, this->Empty.Successor(this->keys.size()));
}

/**
 * Insert
 *   Returns a pointer to the inserted value if successful, nullptr otherwise.
 */
TYPED_TEST_P(TreeTest, Insert) {
    int newcomer = this->keys.size();

    // Ensure the insertion is successful.
    TypeParam&& v = static_cast<TypeParam&&>(newcomer);
    ASSERT_NE(nullptr, this->Balanced.Insert(newcomer, std::forward<TypeParam>(v)));
    ASSERT_NE(nullptr, this->Branching.Insert(newcomer, std::forward<TypeParam>(v)));
    ASSERT_NE(nullptr, this->Empty.Insert(newcomer, std::forward<TypeParam>(v)));

    // Confirm the existence of inserted value.
    EXPECT_EQ(static_cast<TypeParam>(newcomer), *this->Balanced.Search(newcomer));
    EXPECT_EQ(static_cast<TypeParam>(newcomer), *this->Branching.Search(newcomer));
    EXPECT_EQ(static_cast<TypeParam>(newcomer), *this->Empty.Search(newcomer));
}

/**
 * Delete
 *   Returns the deleted value if successful and -1 otherwise.
 */
TYPED_TEST_P(TreeTest, Delete) {
    int freed = this->keys.front();
    
    TypeParam freedBa = this->Balanced.Delete(freed);
    TypeParam freedBr = this->Branching.Delete(freed);

    // 1. Ensure the deletion is successful.
    ASSERT_NE(static_cast<TypeParam>(-1), freedBa);
    ASSERT_NE(static_cast<TypeParam>(-1), freedBr);
    EXPECT_EQ(static_cast<TypeParam>(freed), freedBa);
    EXPECT_EQ(static_cast<TypeParam>(freed), freedBr);

    // 2. Ensure it is the only missing value.
    for (auto val : this->keys) {
        if (val == freed) {
            TypeParam nonBa = this->Balanced.Delete(val);
            TypeParam nonBr = this->Branching.Delete(val);
            ASSERT_NE(static_cast<TypeParam>(val), nonBa);
            ASSERT_NE(static_cast<TypeParam>(val), nonBr);
            EXPECT_EQ(static_cast<TypeParam>(-1), nonBa);
            EXPECT_EQ(static_cast<TypeParam>(-1), nonBr);
        }
        else {
            EXPECT_EQ(static_cast<TypeParam>(val), this->Balanced.Delete(val));
            EXPECT_EQ(static_cast<TypeParam>(val), this->Branching.Delete(val));
        }
    }

    // 3. Ensure all values have been effectively removed.
    for (auto val : this->keys) {
        TypeParam nonBa = this->Balanced.Delete(val);
        TypeParam nonBr = this->Branching.Delete(val);
        ASSERT_NE(static_cast<TypeParam>(val), nonBa);
        ASSERT_NE(static_cast<TypeParam>(val), nonBr);
        EXPECT_EQ(static_cast<TypeParam>(-1), nonBa);
        EXPECT_EQ(static_cast<TypeParam>(-1), nonBr);
    }

    // Non-existing values.
    EXPECT_EQ(static_cast<TypeParam>(-1), this->Balanced.Delete(this->keys.size()));
    EXPECT_EQ(static_cast<TypeParam>(-1), this->Branching.Delete(this->keys.size()));
    EXPECT_EQ(static_cast<TypeParam>(-1), this->Empty.Delete(this->keys.size()));
}