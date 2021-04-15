#pragma once
#include "..\Tree\Tree.hpp"

// Internals
template<typename T>
class TreeTestStructure : public ::testing::Test {
protected:
    using Tree = Tree<T>;
    using Node = typename Tree::Node;

    void SetUp() override {
        // Test

        int max = 10;
        int branch = max / 2;
        for (int i = 0; i < max / 2; i++) {
            //TODO:
        }


        // TODO: Build from either of two trees
        n1 = allocate(0, std::move(static_cast<T>(7)));
        n2 = allocate(1, std::move(static_cast<T>(9)));
    }
    void TearDown() override {
        delete node;
        node = nullptr;
    }

    inline Node* search(const int k, Node* n = nullptr) const {
        return Test.search(k, std::forward<Node>(n));
    }
    inline Node* minimum(Node* n) const {
        return Test.minimum(std::forward<Node>(n));
    }
    inline Node* maximum(Node* n) const {
        return Test.maximum(std::forward<Node>(n));
    }
    inline Node* predecessor(Node* n) const {
        return Test.predecessor(std::forward<Node>(n));
    }
    inline Node* successor(Node* n) const {
        return Test.successor(std::forward<Node>(n));
    }
    inline void transplant(Node* m, Node* n) {
        return Test.transplant(std::forward<Node>(m), std::forward<Node>(n));
    }
    inline Node* allocate(const int k, T&& v) {
        return Test.allocate(k, std::forward<T>(v));
    }
    inline Node* root() {
        return Test.root;
    }

    Tree Test;
    Node nested;
};

TYPED_TEST_CASE_P(TreeTestStructure);


TYPED_TEST_P(TreeTestStructure, InvariantTree) {
    //    ASSERT_EQ(root(), nullptr);
}

TYPED_TEST_P(TreeTestStructure, InvariantNode) {
    using T = TypeParam;

    Node* n = allocate(node.key, std::move(*node.val));
    ASSERT_EQ(node.key, n->key);
    ASSERT_EQ(*node.val, *n->value);

    ASSERT_EQ(nullptr, n->parent);
    ASSERT_EQ(nullptr, n->right);
    ASSERT_EQ(nullptr, n->left);

    delete n;
}

TYPED_TEST_P(TreeTestStructure, allocate) {
}

TYPED_TEST_P(TreeTestStructure, search) {
}

TYPED_TEST_P(TreeTestStructure, minimum) {
    Node* n = Tree.search(branch);


    delete n;
}

TYPED_TEST_P(TreeTestStructure, maximum) {
    Node* n = Tree.search(branch);


    delete n;
}

TYPED_TEST_P(TreeTestStructure, predecessor) {
    nested;

}

TYPED_TEST_P(TreeTestStructure, successor) {
    nested;
}

TYPED_TEST_P(TreeTestStructure, transplant) {
    // 
    Node* n = Tree.search(nested);
    Node* m = n->parent;
}

REGISTER_TYPED_TEST_CASE_P(TreeTestStructure,
    InvariantTree,
    InvariantNode,
    allocate,
    search,
    minimum,
    maximum,
    predecessor,
    successor,
    transplant);