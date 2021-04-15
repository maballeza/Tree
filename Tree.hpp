#pragma once

/**
* Binary Tree Implementation
*/
template<typename T>
class Tree
{
public:
    ~Tree();
    struct Node {
        Node(const int k, T* v) : key(k), value(v), parent(nullptr), left(nullptr), right(nullptr) {}
        ~Node();
        int key;
        T* value;
        Node* parent;
        Node* left;
        Node* right;
    };
    // Accessors
    T* Search(const int k) const;   // Returns nullptr if the item does not exist.
    T* Minimum() const; // Returns nullptr if the tree is empty.
    T* Maximum() const; // Returns nullptr if the tree is empty.
    T* Predecessor(const int k) const;  // Returns nullptr if no predecessor exists.
    T* Successor(const int k) const;    // Returns nullptr if no successor exists.
    // Modifiers
    T* Insert(const int k, T&& v);
    T Delete(const int k); // Returns -1 if the value does not exist.
private:
    Node* search(const int k, Node* n = nullptr) const; // Also supports both [Prede-/Suc-]cessor() functions.
    Node* minimum(Node* n) const;
    Node* maximum(Node* n) const;
    Node* predecessor(Node* n) const;
    Node* successor(Node* n) const;
    // Establishes mutual parent-child relationship.
    void transplant(Node* m, Node* n);  // Supports Insert().
    // Memory management.
    Node* allocate(const int key, T&& value);

    Node* root{};
};

template<typename T>
Tree<T>::~Tree() {
    if (Node* m = minimum(root)) { // If the tree is non-empty.
        while (m) {
            Node* n = successor(m);
            if (m->parent) {
                if (m->parent->right == m) {
                    m->parent->right = nullptr;
                }
                else {
                    m->parent->left = nullptr;
                }
                m->parent = nullptr;
            }
            if (m->right) {
                m->right->parent = nullptr;
                m->right = nullptr;
            }
            else if (m->left) {
                m->left->parent = nullptr;
                m->left = nullptr;
            }
            delete m;
            if (m == n) break; // If m pointed to the final element.
            m = n;
        }
    }
}

template<typename T>
Tree<T>::Node::~Node() {
    delete value;
    value = nullptr;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::search(const int key, Node* n) const {
    if (n) {
        if (key > n->key) {
            n = search(key, n->right);
        }
        else if (key < n->key) {
            n = search(key, n->left);
        }
    }
    return n;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::minimum(Node* n) const {
    if (n) {
        while (n->left) {
            n = n->left;
        }
    }
    return n;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::maximum(Node* n) const {
    if (n) {
        while (n->right) {
            n = n->right;
        }
    }
    return n;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::predecessor(Node* found) const {
    if (Node* n = found) {
        if (n->left) {
            found = maximum(n->left);
        }
        else {
            while (n->parent) {
                if (n->parent->right == n) {
                    found = n->parent;
                    break;
                }
                n = n->parent;
            }
        }
    }
    return found;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::successor(Node* found) const {
    if (Node* n = found) {
        if (n->right) {
            found = minimum(n->right);
        }
        else {
            while (n->parent) {
                if (n->parent->left == n) {
                    found = n->parent;
                    break;
                }
                n = n->parent;
            }
        }
    }
    return found;
}

template<typename T>
void Tree<T>::transplant(Node* n, Node* m) {
    if (m) {
        m->parent = n->parent;
    }
    if (n->parent == nullptr) {
        root = m;
    }
    else if (n == n->parent->right) {
        n->parent->right = m;
    }
    else {
        n->parent->left = m;
    }
}

template<typename T>
T* Tree<T>::Search(const int k) const {
    Node* found = search(k, root);
    return found != nullptr ? found->value : nullptr;
}

template<typename T>
T* Tree<T>::Minimum() const {
    return root != nullptr ? minimum(root)->value : nullptr;
}

template<typename T>
T* Tree<T>::Maximum() const {
    return root != nullptr ? maximum(root)->value : nullptr;
}

template<typename T>
T* Tree<T>::Predecessor(const int k) const {
    Node* found = search(k, root);
    return found != nullptr ? predecessor(found)->value : nullptr;
}

template<typename T>
T* Tree<T>::Successor(const int k) const {
    Node* found = search(k, root);
    return found != nullptr ? successor(found)->value : nullptr;
}

template<typename T>
T* Tree<T>::Insert(const int k, T&& val) {
    if (Node* insertion = allocate(k, std::forward<T>(val))) {
        if (nullptr == root) {
            root = insertion;
        }
        else {
            Node* m = root;
            Node* n = m;
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
        return insertion->value;
    }
    return nullptr;
}

template<typename T>
T Tree<T>::Delete(const int k) {
    if (Node* found = search(k, root)) {
        if (nullptr == found->left) {
            transplant(found, found->right);
        }
        else if (nullptr == found->right) {
            transplant(found, found->left);
        }
        else {
            Node* m = minimum(found->right);
            if (found != m->parent) {
                transplant(m, m->right);
                m->right = found->right;
                m->right->parent = m;
            }
            transplant(found, m); // Handles parent-child references.
            m->left = found->left;
            m->left->parent = m;
        }
        T value = *found->value;
        delete found;
        return value;
    }
    return T{ -1 };
}

template<typename T>
typename Tree<T>::Node* Tree<T>::allocate(const int key, T&& val) {
    try {
        T* value = new T{ std::forward<T>(val) };
        try {
            Node* node = new Node{ key, value };
            return node;
        }
        catch(std::bad_alloc e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }
    catch(std::bad_alloc e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}