#pragma once

/**
 * Binary Tree
 */
template<typename T>
struct Node {
    Node(int k, T&& v) : key(k), value(v), parent{}, left{}, right{} {}
    Node(const Node& n) : key(n.key), value(n.value), parent(n.parent), left(n.left), right(n.right) {}
    Node& operator=(const Node&);
    ~Node() {}
    int key;
    T value;
private:
    template<typename T> friend class Tree;
    Node* parent;
    Node* left;
    Node* right;
};

template<typename T>
Node<T>& Node<T>::operator=(const Node& n) {
    value = n.value;
    key = n.kay;
    parent = n.parent;
    left = n.left;
    right = n.right;
}

template<typename T>
class Tree {
public:
    Tree() : root{} {}
    ~Tree();
    
    // Modifiers
    Node<T>* Insert(int k, T&& val);
    Node<T> Delete(Node<T>* n);
    // Accessors return nullptr if the requested value does not exist or if the tree is empty.
    Node<T>* Search(int k, Node<T>* n = nullptr) const;
    Node<T>* Minimum(Node<T>* n = nullptr) const;
    Node<T>* Maximum(Node<T>* n = nullptr) const;
    Node<T>* Predecessor(Node<T>* n) const;
    Node<T>* Successor(Node<T>* n) const;
    
private:
    void Transplant(Node<T>* m, Node<T>* n);  // Establishes mutual parent-child relationship; supports Insert().
    Node<T>* Allocate(int k, T&& val);
    
    Node<T>* root;
};

template<typename T>
Tree<T>::~Tree() {
    if (Node<T>* min = Minimum(root)) { // If the tree is non-empty.
        while (Node<T>* n = Successor(min)) {
            if (min->right) {
                min->right->parent = nullptr;   // Successor() dereferences parent
            }
            if (min->parent && min == min->parent->left) {
                min->parent->left = nullptr;    // Successor() calls Minimum() which dereferences left
            }
            delete min;
            min = n;
        }
        delete min;
    }
}

template<typename T>
Node<T>* Tree<T>::Insert(int k, T&& val) {
    if (Node<T>* insertion = Allocate(k, std::forward<T>(val))) {
        if (nullptr == root) {
            root = insertion;
        }
        else {
            Node<T>* m = root;
            Node<T>* n = m;
            while (n) {
                m = n;
                if (n->key <= insertion->key) {
                    n = n->right;
                }
                else {
                    n = n->left;
                }
            }
            if (m->key <= insertion->key) {
                m->right = insertion;
                m->right->parent = m;
            }
            else {
                m->left = insertion;
                m->left->parent = m;
            }
        }
        return insertion;
    }
    return nullptr;
}

template<typename T>
Node<T> Tree<T>::Delete(Node<T>* n) {
    if (n) {
        if (nullptr == n->left) {
            Transplant(n, n->right);
        }
        else if (nullptr == n->right) {
            Transplant(n, n->left);
        }
        else {
            Node<T>* min = Minimum(n->right);
            if (n != min->parent) {
                Transplant(min, min->right);
                min->right = n->right;
                min->right->parent = min;
            }
            Transplant(n, min); // Handles parent-child references.
            min->left = n->left;
            min->left->parent = min;
        }
        Node<T> value = *n;
        delete n;
        return value;
    }
    return Node<T>{ -1, T{} };
}

template<typename T>
Node<T>* Tree<T>::Search(int k, Node<T>* n) const {
    if (n || root) {
        if (!n && root) {
            n = root;
        }
        if (k > n->key && n->right) {
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

template<typename T>
Node<T>* Tree<T>::Minimum(Node<T>* n) const {
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

template<typename T>
Node<T>* Tree<T>::Maximum(Node<T>* n) const {
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

template<typename T>
Node<T>* Tree<T>::Predecessor(Node<T>* found) const {
    if (Node<T>* n = found) {
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

template<typename T>
Node<T>* Tree<T>::Successor(Node<T>* found) const {
    if (Node<T>* n = found) {
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

template<typename T>
void Tree<T>::Transplant(Node<T>* m, Node<T>* n) { 
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

template<typename T>
Node<T>* Tree<T>::Allocate(int k, T&& val) {
    try {
        Node<T>* node = new Node<T>{ k, std::forward<T>(val) };
        return node;
    }
    catch (std::bad_alloc e) {
        std::cerr << "Node allocation error in file " << __FILE__ << " (line " << __LINE__ - 4 << ")." << std::endl;
        return nullptr;
    }
}