#pragma once

template<typename T, typename U>
struct Node {
    U key;
    T item;
private:
    template<typename T, typename U> friend class Tree;
    Node(U k, T&& v) : key(k), item(v), parent{}, left{}, right{} {}
    Node* parent;
    Node* left;
    Node* right;
};

/**
*   Unbalanced Binary Tree
*/
template<typename T, typename U>
class Tree{
public:
    Tree() : root{} {}
    ~Tree();
    
    
    void TreeWalk() const;
    
    /**
    * Modifiers
    */
    void Insert(U k, T&& itm);
    void Delete(Node<T, U>** n);
    
    /**
    * Accessors
    *  Return nullptr if the requested item does not exist or if the tree is empty.
    */
    Node<T, U>* Search(U k, Node<T, U>* n = nullptr) const;
    Node<T, U>* Minimum(Node<T, U>* n = nullptr) const;
    Node<T, U>* Maximum(Node<T, U>* n = nullptr) const;
    Node<T, U>* Predecessor(Node<T, U>* n) const;
    Node<T, U>* Successor(Node<T, U>* n) const;
    
private:
    void Transplant(Node<T, U>* m, Node<T, U>* n);  // Establishes mutual parent-child relationship; supports Insert().
    Node<T, U>* Allocate(U k, T&& itm);
    Node<T, U>* root;
};

template<typename T, typename U>
Tree<T, U>::~Tree() {
    if (Node<T, U>* min = Minimum(root)) { // Deletes by walking up the tree.
        while (Node<T, U>* n = Successor(min)) {
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

template<typename T, typename U>
void Tree<T, U>::TreeWalk() const {
    if (Node<T, U>* min = Minimum(root)) { // Deletes by walking up the tree.
        while (Node<T, U>* n = Successor(min)) {
            std::cout << "{ " << min->key << "    , " << min->item << " }" << '\n';
            min = n;
        }
        std::cout << "{ " << min->key << "\t, " << min->item << " }" << '\n';
    }
}

template<typename T, typename U>
void Tree<T, U>::Insert(U k, T&& itm) {
    if (Node<T, U>* insertion = Allocate(k, std::forward<T>(itm))) {
        if (Node<T, U>* m = root) {
            Node<T, U>* n = m;
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

template<typename T, typename U>
void Tree<T, U>::Delete(Node<T, U>** n) {
    if (Node<T, U>* np = *n) {
        if (nullptr == np->left) {
            Transplant(np, np->right); // Handles parent-child references.
        }
        else if (nullptr == np->right) {
            Transplant(np, np->left);
        }
        else {
            Node<T, U>* min = Minimum(np->right);
            if (np != min->parent) {
                Transplant(min, min->right);
                min->right = np->right;
                min->right->parent = min;
            }
            Transplant(np, min);
            min->left = np->left;
            min->left->parent = min;
        }
        Node<T, U> item = *np;
        delete *n;
        *n = nullptr;
    }
}

template<typename T, typename U>
Node<T, U>* Tree<T, U>::Search(U k, Node<T, U>* n) const {
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

template<typename T, typename U>
Node<T, U>* Tree<T, U>::Minimum(Node<T, U>* n) const {
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

template<typename T, typename U>
Node<T, U>* Tree<T, U>::Maximum(Node<T, U>* n) const {
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

template<typename T, typename U>
Node<T, U>* Tree<T, U>::Predecessor(Node<T, U>* found) const {
    if (Node<T, U>* n = found) {
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

template<typename T, typename U>
Node<T, U>* Tree<T, U>::Successor(Node<T, U>* found) const {
    if (Node<T, U>* n = found) {
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

template<typename T, typename U>
void Tree<T, U>::Transplant(Node<T, U>* m, Node<T, U>* n) { 
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

template<typename T, typename U>
Node<T, U>* Tree<T, U>::Allocate(U k, T&& itm) {
    try {
        return new Node<T, U>{ k, std::forward<T>(itm) };
    }
    catch (std::bad_alloc e) {
        std::cerr << "Node allocation failure on line " << __LINE__ - 3 << " of " << __FILE__ << "." << std::endl;
        return nullptr;
    }
}