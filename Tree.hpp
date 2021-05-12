#pragma once

template<typename K, typename I>
struct Node {
    K key;
    I item;
private:
    template<typename K, typename I> friend class Tree;
    Node(K k, I&& v) : key(k), item(v), parent{}, left{}, right{} {}
    Node* parent;
    Node* left;
    Node* right;
};

/**
*   Unbalanced Binary Tree
*/
template<typename K, typename I>
class Tree {
public:
    Tree() : root{} {}
    ~Tree();
    
    void TreeWalk() const;
    
    /**
    * Modifiers
    */
    void Insert(K k, I&& itm);
    void Delete(Node<K, I>** n);
    
    /**
    * Accessors
    *  Return nullptr if the requested item does not exist or if the tree is empty.
    */
    Node<K, I>* Search(K k, Node<K, I>* n = nullptr) const;
    Node<K, I>* Minimum(Node<K, I>* n = nullptr) const;
    Node<K, I>* Maximum(Node<K, I>* n = nullptr) const;
    Node<K, I>* Predecessor(Node<K, I>* n) const;
    Node<K, I>* Successor(Node<K, I>* n) const;
    
private:
    void Transplant(Node<K, I>* m, Node<K, I>* n);  // Establishes mutual parent-child relationship; supports Insert().
    Node<K, I>* Allocate(K k, I&& itm);
    Node<K, I>* root;
};

template<typename K, typename I>
Tree<K, I>::~Tree() {
    if (Node<K, I>* min = Minimum(root)) { // Deletes by walking up the tree.
        while (Node<K, I>* n = Successor(min)) {
            if (min->right) {
                min->right->parent = nullptr;
            }
            if (nullptr != min->parent && min == min->parent->left) {
                min->parent->left = nullptr;
            }
            delete min;
            min = n;
        }
        delete min;
    }
}

template<typename K, typename I>
void Tree<K, I>::TreeWalk() const {
    if (Node<K, I>* min = Minimum(root)) { // Deletes by walking up the tree.
        while (Node<K, I>* n = Successor(min)) {
            std::cout << "{ " << min->key << "    , " << min->item << " }" << '\n';
            min = n;
        }
        std::cout << "{ " << min->key << "\t, " << min->item << " }" << '\n';
    }
}

template<typename K, typename I>
void Tree<K, I>::Insert(K k, I&& itm) {
    if (Node<K, I>* insertion = Allocate(k, std::forward<I>(itm))) {
        if (Node<K, I>* m = root) {
            Node<K, I>* n = m;
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

template<typename K, typename I>
void Tree<K, I>::Delete(Node<K, I>** n) {
    if (n != nullptr) {
        if (Node<K, I>* np = *n) {
            if (nullptr == np->left) {
                Transplant(np, np->right); // Handles parent-child references.
            }
            else if (nullptr == np->right) {
                Transplant(np, np->left);
            }
            else {
                Node<K, I>* min = Minimum(np->right);
                if (np != min->parent) {
                    Transplant(min, min->right);
                    min->right = np->right;
                    min->right->parent = min;
                }
                Transplant(np, min);
                min->left = np->left;
                min->left->parent = min;
            }
            Node<K, I> item = *np;
            delete* n;
            *n = nullptr;
        }
    }
}

template<typename K, typename I>
Node<K, I>* Tree<K, I>::Search(K k, Node<K, I>* n) const {
    if (n || root) {
        if (!n && root) {
            n = root;
        }
        if (n->key < k && n->right) {
            n = Search(k, n->right);
        }
        else if (k < n->key && n->left) {
            n = Search(k, n->left);
        }
        else if (k != n->key) {
            n = nullptr;
        }
    }
    return n;
}

template<typename K, typename I>
Node<K, I>* Tree<K, I>::Minimum(Node<K, I>* n) const {
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

template<typename K, typename I>
Node<K, I>* Tree<K, I>::Maximum(Node<K, I>* n) const {
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

template<typename K, typename I>
Node<K, I>* Tree<K, I>::Predecessor(Node<K, I>* found) const {
    if (Node<K, I>* n = found) {
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

template<typename K, typename I>
Node<K, I>* Tree<K, I>::Successor(Node<K, I>* found) const {
    if (Node<K, I>* n = found) {
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

template<typename K, typename I>
void Tree<K, I>::Transplant(Node<K, I>* m, Node<K, I>* n) { 
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

template<typename K, typename I>
Node<K, I>* Tree<K, I>::Allocate(K k, I&& itm) {
    try {
        return new Node<K, I>{ k, std::forward<I>(itm) };
    }
    catch (std::bad_alloc e) {
        std::cerr << "Node allocation failure on line " << __LINE__ - 3 << " of " << __FILE__ << "." << std::endl;
        return nullptr;
    }
}