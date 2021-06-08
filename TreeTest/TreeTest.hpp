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
* Move Constructor
*   Clones "moved-from" tree values, deallocating memory.
*/
TYPED_TEST_P(TreeTest, MoveConstructor) {
    using I = TypeParam;
    using Node = typename Tree<int, I>::Node;

    Tree<int, I> tr{ std::move(this->BalancedTr) };

    // Tree "moved-to."
    Node* n = tr.Minimum();
    for (auto& k : this->keys) {
        EXPECT_EQ(k, n->item);
        n = this->BalancedTr.Successor(n);
    }
    
    // Tree "moved-from."
    EXPECT_EQ(nullptr, this->BalancedTr.Maximum());
    EXPECT_EQ(nullptr, this->BalancedTr.Minimum());
    for (auto& k : this->keys) {
        EXPECT_EQ(nullptr, this->BalancedTr.Search(k));
    }
}

/**
* Destructor
*   Deallocates memory.
*/
TYPED_TEST_P(TreeTest, Destructor) {
    using I = TypeParam;

    Tree<int, I>* trP;
    {
        Tree<int, I> tr{ std::move(this->BalancedTr) };
        trP = &tr;
    }
    EXPECT_EQ(nullptr, trP->Minimum());
    EXPECT_EQ(nullptr, trP->Maximum());
}

/**
* Search
*   Returns either a node pointer to the corresponding value or nullptr.
*/
TYPED_TEST_P(TreeTest, Search) {
    using I = TypeParam;
    using Node = typename Tree<int, I>::Node;

    for (auto& k : this->keys) {
        Node* pBa = this->BalancedTr.Search(k);
        Node* pBr = this->BranchingTr.Search(k);
        EXPECT_EQ(static_cast<I>(k), pBa->item);
        EXPECT_EQ(static_cast<I>(k), pBr->item);
    }

    // Consistency
    for (auto& k : this->keys) {
        Node* p1 = this->BalancedTr.Search(k);
        Node* p2 = this->BalancedTr.Search(k);
        EXPECT_EQ(&p1->item, &p2->item);
    }

    // Non-Existent Value
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
    using I = TypeParam;
    using Node = typename Tree<int, I>::Node;

    for (auto& k : this->keys) { // 'keys' contains key values in ascending order.
        // 1. Find the node with the maximum value.
        Node* pBa = this->BalancedTr.Search(k);
        Node* pBr = this->BranchingTr.Search(k);

        // 2. Confirm it is the correct one.
        EXPECT_EQ(pBa, this->BalancedTr.Minimum());
        EXPECT_EQ(pBr, this->BranchingTr.Minimum());

        // 3. Remove it for iterative testing.
        this->BalancedTr.Delete(&pBa);
        this->BranchingTr.Delete(&pBr);
    }
    
    // Minimum does not exist for an empty tree.
    EXPECT_EQ(nullptr, this->EmptyTr.Minimum());
}

TYPED_TEST_P(TreeTest, Maximum) {
    using I = TypeParam;
    using Node = typename Tree<int, I>::Node;

    for (auto& k : this->rkeys) { // 'rkeys' contains key values in descending order.
        // 1. Find the node with the maximum value.
        Node* pBa = this->BalancedTr.Search(k);
        Node* pBr = this->BranchingTr.Search(k);

        // 2. Confirm it is the correct one.
        EXPECT_EQ(pBa, this->BalancedTr.Maximum());
        EXPECT_EQ(pBr, this->BranchingTr.Maximum());

        // 3. Remove it for iterative testing.
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
    using I = TypeParam;
    using Node = typename Tree<int, I>::Node;

    // Confirm a predecessor does not exist only for the smallest value.
    for (auto& k : this->keys) {
        Node* pBa_1 = this->BalancedTr.Search(k);
        Node* pBr_1 = this->BranchingTr.Search(k);
        if (k == this->keys.front()) {
            EXPECT_EQ(nullptr, this->BalancedTr.Predecessor(pBa_1));
            EXPECT_EQ(nullptr, this->BranchingTr.Predecessor(pBr_1));
        }
        else {
            Node* pBa_0 = this->BalancedTr.Search(k - 1);
            Node* pBr_0 = this->BranchingTr.Search(k - 1);
            EXPECT_EQ(pBa_0, this->BalancedTr.Predecessor(pBa_1));
            EXPECT_EQ(pBr_0, this->BranchingTr.Predecessor(pBr_1));
        }
    }
    
    int arbitrary = this->keys.size();
    Node* pArbitrary = this->EmptyTr.Search(arbitrary);
    EXPECT_EQ(nullptr, this->EmptyTr.Predecessor(pArbitrary));
}

TYPED_TEST_P(TreeTest, Successor) {
    using I = TypeParam;
    using Node = typename Tree<int, I>::Node;

    // Confirm a successor does not exist only for the greatest value.
    for (auto& k : this->keys) {
        Node* pBa_0 = this->BalancedTr.Search(k);
        Node* pBr_0 = this->BranchingTr.Search(k);
        if (k == this->keys.back()) {
            EXPECT_EQ(nullptr, this->BalancedTr.Successor(pBa_0));
            EXPECT_EQ(nullptr, this->BranchingTr.Successor(pBr_0));
        }
        else {
            Node* pBa_1 = this->BalancedTr.Search(k + 1);
            Node* pBr_1 = this->BranchingTr.Search(k + 1);
            EXPECT_EQ(pBa_1, this->BalancedTr.Successor(pBa_0));
            EXPECT_EQ(pBr_1, this->BranchingTr.Successor(pBr_0));
        }
    }

    int arbitrary = this->keys.size();
    Node* pArbitrary = this->EmptyTr.Search(arbitrary);
    EXPECT_EQ(nullptr, this->EmptyTr.Successor(pArbitrary));
}

/**
* Insert
*   Inserts the new value if memory allocation is successful.
*/
TYPED_TEST_P(TreeTest, Insert) {
    using I = TypeParam;

    // Insert the new value.
    int arbitrary = this->keys.size();
    this->BalancedTr.Insert(arbitrary, static_cast<I&&>(arbitrary));
    this->BranchingTr.Insert(arbitrary, static_cast<I&&>(arbitrary));
    this->EmptyTr.Insert(arbitrary, static_cast<I&&>(arbitrary));

    // Confirm the existence of inserted value.
    EXPECT_EQ(static_cast<I>(arbitrary), this->BalancedTr.Search(arbitrary)->item);
    EXPECT_EQ(static_cast<I>(arbitrary), this->BranchingTr.Search(arbitrary)->item);
    EXPECT_EQ(static_cast<I>(arbitrary), this->EmptyTr.Search(arbitrary)->item);
}

/**
* Delete
*   Deletes the corresponding node if it is found.
*/
TYPED_TEST_P(TreeTest, Delete) {
    using I = TypeParam;
    using Node = typename Tree<int, I>::Node;

    // 1. Obtain a node pointer to the indicated value for removal.
    int freed = this->keys.front();
    Node* pFBa = this->BalancedTr.Search(freed);
    Node* pFBr = this->BranchingTr.Search(freed);

    // 2. Remove the node.
    this->BalancedTr.Delete(&pFBa);
    this->BranchingTr.Delete(&pFBr);
    EXPECT_EQ(nullptr, pFBa);
    EXPECT_EQ(nullptr, pFBr);

    // 3. Ensure it is the only one missing.
    for (auto& k : this->keys) {
        Node* pValBa = this->BalancedTr.Search(k);
        Node* pValBr = this->BranchingTr.Search(k);
        if (k == freed) {
            EXPECT_EQ(nullptr, pValBa);
            EXPECT_EQ(nullptr, pValBr);
        }
    }

    // 4. Remove remaining nodes, ensuring they've been effectively deleted.
    for (auto& k : this->keys) {
        pFBa = this->BalancedTr.Search(k);
        pFBr = this->BranchingTr.Search(k);
        if (k != freed) {
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