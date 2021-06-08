#include "TreeTestString.hpp"

/**
* Move Constructor
*   Clones "moved-from" tree values, deallocating memory.
*/
TEST_F(TreeTestString, MoveConstructor) {
    using Node = typename Tree<short, std::wstring>::Node;

    Tree<short, std::wstring> tr{ std::move(BalancedTr) };

    // Tree "moved-to."
    Node* n = tr.Minimum();
    for (auto& v : keys) {
        EXPECT_EQ(std::to_wstring(v), n->item);
        n = BalancedTr.Successor(n);
    }

    // Tree "moved-from."
    EXPECT_EQ(nullptr, BalancedTr.Maximum());
    EXPECT_EQ(nullptr, BalancedTr.Minimum());
    for (auto& v : keys) {
        EXPECT_EQ(nullptr, BalancedTr.Search(v));
    }
}

/**
* Destructor
*   Deallocates memory.
*/
TEST_F(TreeTestString, Destructor) {
    Tree<short, std::wstring>* trp;
    {
        Tree<short, std::wstring> tr{ std::move(BalancedTr) };
        trp = &tr;
        EXPECT_EQ(tr.Minimum()->item, trp->Minimum()->item);
        EXPECT_EQ(tr.Maximum()->item, trp->Maximum()->item);
    }

    EXPECT_EQ(nullptr, trp->Minimum());
    EXPECT_EQ(nullptr, trp->Maximum());
}

/**
* Search
*   Returns either a node pointer to the corresponding value or nullptr.
*/
TEST_F(TreeTestString, Search) {
    using Node = typename Tree<short, std::wstring>::Node;

    for (auto& k : keys) {
        Node* pBa = BalancedTr.Search(k);
        Node* pBr = BranchingTr.Search(k);
        EXPECT_EQ(std::to_wstring(k), pBa->item);
        EXPECT_EQ(std::to_wstring(k), pBr->item);
    }

    for (auto& k : keys) {
        Node* p1 = BalancedTr.Search(k);
        Node* p2 = BalancedTr.Search(k);
        EXPECT_EQ(&p1->item, &p2->item);
    }

    short arbitrary = keys.size();
    EXPECT_EQ(nullptr, BalancedTr.Search(arbitrary));
    EXPECT_EQ(nullptr, BranchingTr.Search(arbitrary));
    EXPECT_EQ(nullptr, EmptyTr.Search(arbitrary));
}

/**
* Minimum & Maximum
*   Returns either a valid node pointer to the corresponding value or nullptr.
*/
TEST_F(TreeTestString, Minimum) {
    using Node = typename Tree<short, std::wstring>::Node;

    for (auto& k : keys) {// keys contains key values in ascending order.
        // 1. Find the node with the maximum value.
        Node* pBa = BalancedTr.Search(k);
        Node* pBr = BranchingTr.Search(k);

        // 2. Confirm it is the correct one.
        EXPECT_EQ(pBa, BalancedTr.Minimum());
        EXPECT_EQ(pBr, BranchingTr.Minimum());

        // 3. Remove it for testing during the following iteration.
        BalancedTr.Delete(&pBa);
        BranchingTr.Delete(&pBr);
    }

    // Minimum does not exist for an empty tree.
    EXPECT_EQ(nullptr, EmptyTr.Minimum());
}

TEST_F(TreeTestString, Maximum) {
    using Node = typename Tree<short, std::wstring>::Node;

    for (auto& k : rkeys) { // rkeys contains key values in descending order.
        // 1. Find the node with the maximum value.
        Node* pBa = BalancedTr.Search(k);
        Node* pBr = BranchingTr.Search(k);

        // 2. Confirm it is the correct one.
        EXPECT_EQ(pBa, BalancedTr.Maximum());
        EXPECT_EQ(pBr, BranchingTr.Maximum());

        // 3. Remove it for testing during the following iteration.
        BalancedTr.Delete(&pBa);
        BranchingTr.Delete(&pBr);
    }

    // Maximum does not exist for an empty tree.
    EXPECT_EQ(nullptr, EmptyTr.Maximum());
}

/**
* Predecessor & Successor
*   Returns either a valid node pointer to the corresponding value or nullptr.
*/
TEST_F(TreeTestString, Predecessor) {
    using Node = typename Tree<short, std::wstring>::Node;

    for (auto& k : keys) {
        Node* pBa_1 = BalancedTr.Search(k);
        Node* pBr_1 = BranchingTr.Search(k);
        if (k == keys.front()) {
            EXPECT_EQ(nullptr, BalancedTr.Predecessor(pBa_1));
            EXPECT_EQ(nullptr, BranchingTr.Predecessor(pBr_1));
        }
        else {
            Node* pBa_0 = BalancedTr.Search(k - 1);
            Node* pBr_0 = BranchingTr.Search(k - 1);
            EXPECT_EQ(pBa_0, BalancedTr.Predecessor(pBa_1));
            EXPECT_EQ(pBr_0, BranchingTr.Predecessor(pBr_1));
        }
    }

    short arbitrary = keys.size();
    Node* pArbitrary = EmptyTr.Search(arbitrary);
    EXPECT_EQ(nullptr, EmptyTr.Predecessor(pArbitrary));
}

TEST_F(TreeTestString, Successor) {
    using Node = typename Tree<short, std::wstring>::Node;

    for (auto& k : keys) {
        Node* pBa_0 = BalancedTr.Search(k);
        Node* pBr_0 = BranchingTr.Search(k);
        if (k == keys.back()) {
            EXPECT_EQ(nullptr, BalancedTr.Successor(pBa_0));
            EXPECT_EQ(nullptr, BranchingTr.Successor(pBr_0));
        }
        else {
            Node* pBa_1 = BalancedTr.Search(k + 1);
            Node* pBr_1 = BranchingTr.Search(k + 1);
            EXPECT_EQ(pBa_1, BalancedTr.Successor(pBa_0));
            EXPECT_EQ(pBr_1, BranchingTr.Successor(pBr_0));
        }
    }

    short arbitrary = keys.size();
    Node* pArbitrary = EmptyTr.Search(arbitrary);
    EXPECT_EQ(nullptr, EmptyTr.Successor(pArbitrary));
}

/**
* Insert
*   Inserts the new value if memory allocation is successful.
*/
TEST_F(TreeTestString, Insert) {
    // Insert the new value.
    short arbitrary = keys.size();
    BalancedTr.Insert(arbitrary, std::move(std::to_wstring(arbitrary)));
    BranchingTr.Insert(arbitrary, std::move(std::to_wstring(arbitrary)));
    EmptyTr.Insert(arbitrary, std::move(std::to_wstring(arbitrary)));

    // Confirm the existence of inserted value.
    EXPECT_EQ(std::to_wstring(arbitrary), BalancedTr.Search(arbitrary)->item);
    EXPECT_EQ(std::to_wstring(arbitrary), BranchingTr.Search(arbitrary)->item);
    EXPECT_EQ(std::to_wstring(arbitrary), EmptyTr.Search(arbitrary)->item);

    EXPECT_NE(nullptr, BalancedTr.Search(arbitrary));
    EXPECT_NE(nullptr, BranchingTr.Search(arbitrary));
    EXPECT_NE(nullptr, EmptyTr.Search(arbitrary));
}

/**
* Delete
*   Deletes the corresponding node if it is found.
*/
TEST_F(TreeTestString, Delete) {
    using Node = typename Tree<short, std::wstring>::Node;

    // 1. Obtain a node pointer to the indicated value.
    short freed = keys.front();
    Node* pFBa = BalancedTr.Search(freed);
    Node* pFBr = BranchingTr.Search(freed);

    // 2. Remove the node.
    std::wstring deletedBaItem = pFBa->item;
    std::wstring deletedBrItem = pFBr->item;
    BalancedTr.Delete(&pFBa);
    BranchingTr.Delete(&pFBr);
    EXPECT_EQ(nullptr, pFBa);
    EXPECT_EQ(nullptr, pFBr);

    // 3. Ensure it is the only one missing.
    for (auto& k : keys) {
        Node* pValBa = BalancedTr.Search(k);
        Node* pValBr = BranchingTr.Search(k);
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
    for (auto& k : keys) {
        pFBa = BalancedTr.Search(k);
        pFBr = BranchingTr.Search(k);
        if (k != freed) {
            EXPECT_NE(nullptr, pFBa);
            EXPECT_NE(nullptr, pFBr);
            BalancedTr.Delete(&pFBa);
            BranchingTr.Delete(&pFBr);
            EXPECT_EQ(nullptr, pFBa);
            EXPECT_EQ(nullptr, pFBr);
        }
    }

    // Non-existent values.
    BalancedTr.Delete(nullptr);
    BranchingTr.Delete(nullptr);
    EmptyTr.Delete(nullptr);
}