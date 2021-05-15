#pragma once


/**
*   Unbalanced Binary Tree
*/
template<typename K, typename I>
class Tree {
public:
    struct Node {
        K key;
        I item;
    private:
        friend class Tree;
        Node(K k, I&& v) : key(k), item(v), parent{}, left{}, right{} {}
        Node* parent;
        Node* left;
        Node* right;
    };

    Tree() : root{} {}
    ~Tree();
    
    void TreeWalk() const;
    
    /**
    * Modifiers
    */
    void Insert(K k, I&& itm);
    void Delete(Node** n);
    
    /**
    * Accessors
    *  Return nullptr if the requested item does not exist or if the tree is empty.
    */
    Node* Search(K k, Node* n = nullptr) const;
    Node* Minimum(Node* n = nullptr) const;
    Node* Maximum(Node* n = nullptr) const;
    Node* Predecessor(Node* n) const;
    Node* Successor(Node* n) const;
    
private:
    void Transplant(Node* m, Node* n);  // Establishes mutual parent-child relationship; supports Insert().
    Node* Allocate(K k, I&& itm);
    Node* root;
};

template<typename K, typename I>
Tree<K, I>::~Tree() {
    if (Node* min = Minimum(root)) { // Deletes by walking up the tree.
        while (Node* n = Successor(min)) {
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
    if (Node* min = Minimum(root)) { // Deletes by walking up the tree.
        while (Node* n = Successor(min)) {
            std::cout << "{ " << min->key << "    , " << min->item << " }" << '\n';
            min = n;
        }
        std::cout << "{ " << min->key << "\t, " << min->item << " }" << '\n';
    }
}

template<typename K, typename I>
void Tree<K, I>::Insert(K k, I&& itm) {
    if (Node* insertion = Allocate(k, std::forward<I>(itm))) {
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

template<typename K, typename I>
void Tree<K, I>::Delete(Node** n) {
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
            Node item = *np;
            delete* n;
            *n = nullptr;
        }
    }
}

template<typename K, typename I>
typename Tree<K, I>::Node* Tree<K, I>::Search(K k, Node* n) const {
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

template<typename K, typename I>
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

template<typename K, typename I>
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

template<typename K, typename I>
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

template<typename K, typename I>
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

template<typename K, typename I>
typename Tree<K, I>::Node* Tree<K, I>::Allocate(K k, I&& itm) {
    try {
        return new Node{ k, std::forward<I>(itm) };
    }
    catch (std::bad_alloc e) {
        std::cerr << "Node allocation failure on line " << __LINE__ - 3 << " of " << __FILE__ << "." << std::endl;
        return nullptr;
    }
}