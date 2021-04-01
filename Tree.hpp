#pragma once

/**
* Binary Tree Implementation
*/
template<typename T>
class Tree
{
public:
    T* Search(int k);
    T* Min();
    T* Max();
    T* Predecessor(int k);
    T* Successor(int k);
private:
    struct Node {
        int key;
        T value;
        Node* parent;
        Node* left;
        Node* right;
    };
};