#pragma once
#include "Node.hpp"
#include <vector>

/**
*   Unbalanced Binary Tree
*/
template <typename K, class I>
class Tree {
public:
    struct Node : BaseNode<I> {
        K key;
    private:
        Node(K k, I&& i) : BaseNode<I>(std::forward<I>(i)), key{ k }, parent{}, left{}, right{} {}
        Node* parent;
        Node* left;
        Node* right;
        friend class Tree;
    };
    
    Tree() : root{} {};
    Tree(Tree&& t) noexcept;
    ~Tree();
        
    /**
    * Modifiers
    */
    void Insert(K k, I&& i);
    void Delete(Node** n) noexcept;
    
    /**
    * Accessors
    *  Return nullptr if the requested item does not exist or if the tree is empty.
    */
    Node* operator[](K k) { return Search(k); }
    
    Node* Search(K k, Node* n = nullptr) const;
    Node* Minimum(Node* n = nullptr) const;
    Node* Maximum(Node* n = nullptr) const;
    Node* Predecessor(Node* n) const;
    Node* Successor(Node* n) const;
    
    std::vector<std::pair<K, I>> Walk() const;

private:
    Node* Allocate(K k, I&& i);
    void DeallocateTree(Node** n) noexcept;
    void Clone(Node* n);
    void Transplant(Node* m, Node* n);  // Establishes mutual parent-child relationship; supports Insert().
    Node* root;
};

template <typename K, typename I>
Tree<K, I>::Tree(Tree&& t) noexcept {
    Clone(t.root);
    t.~Tree();
}

template <typename K, class I>
Tree<K, I>::~Tree() {
    if (Node* min = Minimum(root)) { // Deletes by walking up the tree.
        DeallocateTree(&min);
    }
    root = nullptr;
}

template <typename K, class I>
std::vector<std::pair<K, I>> Tree<K, I>::Walk() const {
    std::vector<std::pair<K, I>> v;
    for (Node* n = Minimum(root); n; n = Successor(n)) {
        v.emplace_back(n->key, n->item);
    }
    return v;
}

template <typename K, class I>
void Tree<K, I>::Insert(K key, I&& item) {
    if (Node* insertion = Allocate(key, std::forward<I>(item))) {
        if (Node* m = root) {
            Node* n = m;
            while (n) {
                m = n;
                if (insertion->key < n->key) {
                    n = n->left;
                }
                else {
                    n = n->right;
                }
            }
            if (insertion->key < m->key) {
                m->left = insertion;
                m->left->parent = m;
            }
            else {
                m->right = insertion;
                m->right->parent = m;
            }
        }
        else {
            root = insertion;
        }
    }
}

template <typename K, class I>
void Tree<K, I>::Delete(Node** n) noexcept {
    if (n != nullptr) {
        if (Node* np = *n) {
            if (nullptr == np->left) {
                Transplant(np, np->right); // Handles parent-child references.
            }
            else if (nullptr == np->right) {
                Transplant(np, np->left);
            }
            else {
                Node* min = Minimum(np->right);
                if (np != min->parent) {
                    Transplant(min, min->right);
                    min->right = np->right;
                    min->right->parent = min;
                }
                Transplant(np, min);
                min->left = np->left;
                min->left->parent = min;
            }
            delete* n;
            *n = nullptr;
        }
    }
}

template <typename K, class I>
typename Tree<K, I>::Node* Tree<K, I>::Search(K key, Node* n) const {
    if (n || root) {
        if (!n && root) {
            n = root;
        }
        if (n->key < key && n->right) {
            n = Search(key, n->right);
        }
        else if (key < n->key && n->left) {
            n = Search(key, n->left);
        }
        else if (key != n->key) {
            n = nullptr;
        }
    }
    return n;
}

template <typename K, class I>
typename Tree<K, I>::Node* Tree<K, I>::Minimum(Node* n) const {
    if (n || root) {
        if (!n) {
            n = root;
        }
        while (n->left) {
            n = n->left;
        }
    }
    return n;
}

template <typename K, class I>
typename Tree<K, I>::Node* Tree<K, I>::Maximum(Node* n) const {
    if (n || root) {
        if (!n) {
            n = root;
        }
        while (n->right) {
            n = n->right;
        }
    }
    return n;
}

template <typename K, class I>
typename Tree<K, I>::Node* Tree<K, I>::Predecessor(Node* found) const {
    if (Node* n = found) {
        if (n->left) {
            found = Maximum(n->left);
        }
        else {
            while (n->parent) {
                if (n == n->parent->right) {
                    break;
                }
                n = n->parent;
            }
            found = n->parent;
        }
    }
    return found;
}

template <typename K, class I>
typename Tree<K, I>::Node* Tree<K, I>::Successor(Node* found) const {
    if (Node* n = found) {
        if (n->right) {
            found = Minimum(n->right);
        }
        else {
            while (n->parent) {
                if (n == n->parent->left) {
                    break;
                }
                n = n->parent;
            }
            found = n->parent;
        }
    }
    return found;
}

template <typename K, class I>
typename Tree<K, I>::Node* Tree<K, I>::Allocate(K key, I&& item) {
    try {
        return new Node{ key, std::forward<I>(item) };
    }
    catch (std::bad_alloc& e) {
        std::cerr << "Node allocation failure on line " << __LINE__ - 3 << " of " << __FILE__ << "." << std::endl;
        return nullptr;
    }
}

template <typename K, typename I>
void Tree<K, I>::DeallocateTree(Node** n) noexcept {
    if (Node* m = *n; m = Successor(m)) {
        DeallocateTree(&m);
    }
    delete* n;
    *n = nullptr;
}

template <typename K, typename I>
void Tree<K, I>::Clone(Node* n) {
    if (n) {
        Insert(n->key, std::move(n->item));
        Clone(n->left);
        Clone(n->right);
    }
}

template <typename K, class I>
void Tree<K, I>::Transplant(Node* m, Node* n) { 
    if (n) {
        n->parent = m->parent;
    }
    if (nullptr == m->parent) {
        root = n;
    }
    else if (m == m->parent->right) {
        m->parent->right = n;
    }
    else {
        m->parent->left = n;
    }
}