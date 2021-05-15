#pragma once
#include "../Tree.hpp"

/**
* class TreeTest
*   Type parameterized test for the Tree interface.
*/
template<typename I>
class TreeTest : public testing::Test {
protected:
    void SetUp() override {
        int Ba[10] = { 5, 6, 7, 8, 9, 4, 3, 2, 1, 0 };
        int Br[10] = { 5, 4, 2, 3, 1, 0, 6, 9, 7, 8 };

        for (auto& k : keys) {
            BalancedTr.Insert(Ba[k], static_cast<I&&>(Ba[k]));
        }
        for (auto& k : keys) {
            BranchingTr.Insert(Br[k], static_cast<I&&>(Br[k]));
        }
    }

    Tree<int, I> EmptyTr;
    Tree<int, I> BalancedTr;
    Tree<int, I> BranchingTr;

    const std::vector<int>  keys{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const std::vector<int> rkeys{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
};

TYPED_TEST_SUITE_P(TreeTest);

/**
* Search
*   Returns either a node pointer to the corresponding value or nullptr.
*/
TYPED_TEST_P(TreeTest, Search) {
    using T = TypeParam;
    
    for (auto& k : this->keys) {
        typename Tree<int, T>::Node* pBa = this->BalancedTr.Search(k);
        typename Tree<int, T>::Node* pBr = this->BranchingTr.Search(k);
        EXPECT_EQ(static_cast<T>(k), pBa->item);
        EXPECT_EQ(static_cast<T>(k), pBr->item);
    }

    for (auto& k : this->keys) {
        typename Tree<int, T>::Node* p1 = this->BalancedTr.Search(k);
        typename Tree<int, T>::Node* p2 = this->BalancedTr.Search(k);
        EXPECT_EQ(&p1->item, &p2->item);
    }

    int arbitrary = this->keys.size();
    EXPECT_EQ(nullptr, this->BalancedTr.Search(arbitrary));
    EXPECT_EQ(nullptr, this->BranchingTr.Search(arbitrary));
    EXPECT_EQ(nullptr, this->EmptyTr.Search(arbitrary));
}

/**
* Minimum & Maximum
*   Returns either a valid node pointer to the corresponding value or nullptr.
*/
TYPED_TEST_P(TreeTest, Minimum) {
    using T = TypeParam;
    
    for (auto& k : this->keys) {// keys contains key values in ascending order.
        // 1. Find the node with the maximum value.
        typename Tree<int, T>::Node* pBa = this->BalancedTr.Search(k);
        typename Tree<int, T>::Node* pBr = this->BranchingTr.Search(k);

        // 2. Confirm it is the correct one.
        EXPECT_EQ(pBa, this->BalancedTr.Minimum());
        EXPECT_EQ(pBr, this->BranchingTr.Minimum());

        // 3. Remove it for testing during the following iteration.
        this->BalancedTr.Delete(&pBa);
        this->BranchingTr.Delete(&pBr);
    }
    
    // Minimum does not exist for an empty tree.
    EXPECT_EQ(nullptr, this->EmptyTr.Minimum());
}

TYPED_TEST_P(TreeTest, Maximum) {
    using T = TypeParam;
    
    for (auto& k : this->rkeys) { // rkeys contains key values in descending order.
        // 1. Find the node with the maximum value.
        typename Tree<int, T>::Node* pBa = this->BalancedTr.Search(k);
        typename Tree<int, T>::Node* pBr = this->BranchingTr.Search(k);

        // 2. Confirm it is the correct one.
        EXPECT_EQ(pBa, this->BalancedTr.Maximum());
        EXPECT_EQ(pBr, this->BranchingTr.Maximum());

        // 3. Remove it for testing during the following iteration.
        this->BalancedTr.Delete(&pBa);
        this->BranchingTr.Delete(&pBr);
    }
    
    // Maximum does not exist for an empty tree.
    EXPECT_EQ(nullptr, this->EmptyTr.Maximum());
}

/**
* Predecessor & Successor
*   Returns either a valid node pointer to the corresponding value or nullptr.
*/
TYPED_TEST_P(TreeTest, Predecessor) {
    using T = TypeParam;

    for (auto& k : this->keys) {
        typename Tree<int, T>::Node* pBa_1 = this->BalancedTr.Search(k);
        typename Tree<int, T>::Node* pBr_1 = this->BranchingTr.Search(k);
        if (k == this->keys.front()) {
            EXPECT_EQ(nullptr, this->BalancedTr.Predecessor(pBa_1));
            EXPECT_EQ(nullptr, this->BranchingTr.Predecessor(pBr_1));
        }
        else {
            typename Tree<int, T>::Node* pBa_0 = this->BalancedTr.Search(k - 1);
            typename Tree<int, T>::Node* pBr_0 = this->BranchingTr.Search(k - 1);
            EXPECT_EQ(pBa_0, this->BalancedTr.Predecessor(pBa_1));
            EXPECT_EQ(pBr_0, this->BranchingTr.Predecessor(pBr_1));
        }
    }
    
    int arbitrary = this->keys.size();
    typename Tree<int, T>::Node* pArbitrary = this->EmptyTr.Search(arbitrary);
    EXPECT_EQ(nullptr, this->EmptyTr.Predecessor(pArbitrary));
}

TYPED_TEST_P(TreeTest, Successor) {
    using T = TypeParam;

    for (auto& k : this->keys) {
        typename Tree<int, T>::Node* pBa_0 = this->BalancedTr.Search(k);
        typename Tree<int, T>::Node* pBr_0 = this->BranchingTr.Search(k);
        if (k == this->keys.back()) {
            EXPECT_EQ(nullptr, this->BalancedTr.Successor(pBa_0));
            EXPECT_EQ(nullptr, this->BranchingTr.Successor(pBr_0));
        }
        else {
            typename Tree<int, T>::Node* pBa_1 = this->BalancedTr.Search(k + 1);
            typename Tree<int, T>::Node* pBr_1 = this->BranchingTr.Search(k + 1);
            EXPECT_EQ(pBa_1, this->BalancedTr.Successor(pBa_0));
            EXPECT_EQ(pBr_1, this->BranchingTr.Successor(pBr_0));
        }
    }

    int arbitrary = this->keys.size();
    typename Tree<int, T>::Node* pArbitrary = this->EmptyTr.Search(arbitrary);
    EXPECT_EQ(nullptr, this->EmptyTr.Successor(pArbitrary));
}

/**
* Insert
*   Inserts the new value if memory allocation is successful.
*/
TYPED_TEST_P(TreeTest, Insert) {
    using T = TypeParam;

    // Insert the new value.
    int arbitrary = this->keys.size();
    this->BalancedTr.Insert(arbitrary, static_cast<T&&>(arbitrary));
    this->BranchingTr.Insert(arbitrary, static_cast<T&&>(arbitrary));
    this->EmptyTr.Insert(arbitrary, static_cast<T&&>(arbitrary));

    // Confirm the existence of inserted value.
    EXPECT_EQ(static_cast<T>(arbitrary), this->BalancedTr.Search(arbitrary)->item);
    EXPECT_EQ(static_cast<T>(arbitrary), this->BranchingTr.Search(arbitrary)->item);
    EXPECT_EQ(static_cast<T>(arbitrary), this->EmptyTr.Search(arbitrary)->item);
    
    EXPECT_NE(nullptr, this->BalancedTr.Search(arbitrary));
    EXPECT_NE(nullptr, this->BranchingTr.Search(arbitrary));
    EXPECT_NE(nullptr, this->EmptyTr.Search(arbitrary));
}

/**
* Delete
*   Deletes the corresponding node if it is found.
*/
TYPED_TEST_P(TreeTest, Delete) {
    using T = TypeParam;
    
    // 1. Obtain a node pointer to the indicated value.
    int freed = this->keys.front();
    typename Tree<int, T>::Node* pFBa = this->BalancedTr.Search(freed);
    typename Tree<int, T>::Node* pFBr = this->BranchingTr.Search(freed);

    // 2. Remove the node.
    T deletedBaItem = pFBa->item;
    T deletedBrItem = pFBr->item;
    this->BalancedTr.Delete(&pFBa);
    this->BranchingTr.Delete(&pFBr);
    EXPECT_EQ(nullptr, pFBa);
    EXPECT_EQ(nullptr, pFBr);

    // 3. Ensure it is the only one missing.
    for (auto& k : this->keys) {
        typename Tree<int, T>::Node* pValBa = this->BalancedTr.Search(k);
        typename Tree<int, T>::Node* pValBr = this->BranchingTr.Search(k);
        if (k == freed) {
            EXPECT_EQ(nullptr, pValBa);
            EXPECT_EQ(nullptr, pValBr);
        }
        else {
            EXPECT_NE(nullptr, pValBa);
            EXPECT_NE(nullptr, pValBr);
        }
    }

    // 4. Remove remaining nodes, ensuring they've been effectively deleted.
    for (auto& k : this->keys) {
        pFBa = this->BalancedTr.Search(k);
        pFBr = this->BranchingTr.Search(k);
        if (k != freed) {
            EXPECT_NE(nullptr, pFBa);
            EXPECT_NE(nullptr, pFBr);
            this->BalancedTr.Delete(&pFBa);
            this->BranchingTr.Delete(&pFBr);
            EXPECT_EQ(nullptr, pFBa);
            EXPECT_EQ(nullptr, pFBr);
        }
    }

    // Non-existent values.
    this->BalancedTr.Delete(nullptr);
    this->BranchingTr.Delete(nullptr);
    this->EmptyTr.Delete(nullptr);
}