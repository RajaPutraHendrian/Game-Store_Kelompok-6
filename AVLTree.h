#ifndef AVLTree_
#define AVLTree_

#include <iostream>
#include <vector>
#include "xcept.h"
using namespace std;

// AVL Tree Node
template<class KeyType, class ValueType>
class AVLNode {
public:
    KeyType key;
    ValueType value;
    int height;
    AVLNode* left;
    AVLNode* right;
    
    AVLNode(const KeyType& k, const ValueType& v)
        : key(k), value(v), height(1), left(0), right(0) {}
};

// AVL Tree (Self-Balancing Binary Search Tree)
template<class KeyType, class ValueType>
class AVLTree {
private:
    AVLNode<KeyType, ValueType>* root;
    
    // Helper functions
    int getHeight(AVLNode<KeyType, ValueType>* node);
    int getBalance(AVLNode<KeyType, ValueType>* node);
    AVLNode<KeyType, ValueType>* rotateRight(AVLNode<KeyType, ValueType>* y);
    AVLNode<KeyType, ValueType>* rotateLeft(AVLNode<KeyType, ValueType>* x);
    AVLNode<KeyType, ValueType>* insertHelper(AVLNode<KeyType, ValueType>* node, 
                                                const KeyType& key, const ValueType& value);
    AVLNode<KeyType, ValueType>* searchHelper(AVLNode<KeyType, ValueType>* node, 
                                                const KeyType& key);
    void inorderHelper(AVLNode<KeyType, ValueType>* node, 
                       void (*visit)(const KeyType&, const ValueType&));
    void reverseInorderHelper(AVLNode<KeyType, ValueType>* node,
                              void (*visit)(const KeyType&, const ValueType&));
    void clearHelper(AVLNode<KeyType, ValueType>* node);
    int countHelper(AVLNode<KeyType, ValueType>* node);
    
    // Collect nodes for getLargestN
    void collectNodesDesc(AVLNode<KeyType, ValueType>* node, 
                          vector<pair<KeyType, ValueType> >& result, int& count, int N);
    
public:
    AVLTree();
    ~AVLTree();
    
    void insert(const KeyType& key, const ValueType& value);
    ValueType* search(const KeyType& key);
    void inorderTraversal(void (*visit)(const KeyType&, const ValueType&));
    void reverseInorderTraversal(void (*visit)(const KeyType&, const ValueType&));
    
    // Get largest N elements (for rankings)
    vector<pair<KeyType, ValueType> > getLargestN(int N);
    
    bool isEmpty() const { return root == 0; }
    int count();
    int height();
    void clear();
};

// Get height of node
template<class KeyType, class ValueType>
int AVLTree<KeyType, ValueType>::getHeight(AVLNode<KeyType, ValueType>* node) {
    if (node == 0) return 0;
    return node->height;
}

// Get balance factor
template<class KeyType, class ValueType>
int AVLTree<KeyType, ValueType>::getBalance(AVLNode<KeyType, ValueType>* node) {
    if (node == 0) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Right rotation
template<class KeyType, class ValueType>
AVLNode<KeyType, ValueType>* AVLTree<KeyType, ValueType>::rotateRight(
    AVLNode<KeyType, ValueType>* y) {
    
    AVLNode<KeyType, ValueType>* x = y->left;
    AVLNode<KeyType, ValueType>* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? 
                     getHeight(y->left) : getHeight(y->right));
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? 
                     getHeight(x->left) : getHeight(x->right));
    
    return x;
}

// Left rotation
template<class KeyType, class ValueType>
AVLNode<KeyType, ValueType>* AVLTree<KeyType, ValueType>::rotateLeft(
    AVLNode<KeyType, ValueType>* x) {
    
    AVLNode<KeyType, ValueType>* y = x->right;
    AVLNode<KeyType, ValueType>* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? 
                     getHeight(x->left) : getHeight(x->right));
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? 
                     getHeight(y->left) : getHeight(y->right));
    
    return y;
}

// Constructor
template<class KeyType, class ValueType>
AVLTree<KeyType, ValueType>::AVLTree() : root(0) {}

// Destructor
template<class KeyType, class ValueType>
AVLTree<KeyType, ValueType>::~AVLTree() {
    clear();
}

// Insert with balancing
template<class KeyType, class ValueType>
AVLNode<KeyType, ValueType>* AVLTree<KeyType, ValueType>::insertHelper(
    AVLNode<KeyType, ValueType>* node, const KeyType& key, const ValueType& value) {
    
    // 1. Normal BST insert
    if (node == 0) {
        return new AVLNode<KeyType, ValueType>(key, value);
    }
    
    if (key < node->key) {
        node->left = insertHelper(node->left, key, value);
    } else if (key > node->key) {
        node->right = insertHelper(node->right, key, value);
    } else {
        // Duplicate key, update value
        node->value = value;
        return node;
    }
    
    // 2. Update height
    node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? 
                        getHeight(node->left) : getHeight(node->right));
    
    // 3. Get balance factor
    int balance = getBalance(node);
    
    // 4. Balance if needed
    
    // Left Left Case
    if (balance > 1 && key < node->left->key) {
        return rotateRight(node);
    }
    
    // Right Right Case
    if (balance < -1 && key > node->right->key) {
        return rotateLeft(node);
    }
    
    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

// Insert public method
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    root = insertHelper(root, key, value);
}

// Search helper
template<class KeyType, class ValueType>
AVLNode<KeyType, ValueType>* AVLTree<KeyType, ValueType>::searchHelper(
    AVLNode<KeyType, ValueType>* node, const KeyType& key) {
    
    if (node == 0) return 0;
    
    if (key == node->key) {
        return node;
    } else if (key < node->key) {
        return searchHelper(node->left, key);
    } else {
        return searchHelper(node->right, key);
    }
}

// Search public method
template<class KeyType, class ValueType>
ValueType* AVLTree<KeyType, ValueType>::search(const KeyType& key) {
    AVLNode<KeyType, ValueType>* result = searchHelper(root, key);
    if (result) {
        return &(result->value);
    }
    return 0;
}

// Inorder traversal (sorted ascending)
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::inorderHelper(
    AVLNode<KeyType, ValueType>* node,
    void (*visit)(const KeyType&, const ValueType&)) {
    
    if (node == 0) return;
    
    inorderHelper(node->left, visit);
    visit(node->key, node->value);
    inorderHelper(node->right, visit);
}

// Reverse inorder traversal (sorted descending)
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::reverseInorderHelper(
    AVLNode<KeyType, ValueType>* node,
    void (*visit)(const KeyType&, const ValueType&)) {
    
    if (node == 0) return;
    
    reverseInorderHelper(node->right, visit);
    visit(node->key, node->value);
    reverseInorderHelper(node->left, visit);
}

// Inorder traversal public
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::inorderTraversal(
    void (*visit)(const KeyType&, const ValueType&)) {
    inorderHelper(root, visit);
}

// Reverse inorder traversal public
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::reverseInorderTraversal(
    void (*visit)(const KeyType&, const ValueType&)) {
    reverseInorderHelper(root, visit);
}

// Collect nodes in descending order (for top N)
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::collectNodesDesc(
    AVLNode<KeyType, ValueType>* node,
    vector<pair<KeyType, ValueType> >& result, int& count, int N) {
    
    if (node == 0 || count >= N) return;
    
    // Right first (largest values)
    collectNodesDesc(node->right, result, count, N);
    
    if (count < N) {
        result.push_back(make_pair(node->key, node->value));
        count++;
    }
    
    collectNodesDesc(node->left, result, count, N);
}

// Get largest N elements
template<class KeyType, class ValueType>
vector<pair<KeyType, ValueType> > AVLTree<KeyType, ValueType>::getLargestN(int N) {
    vector<pair<KeyType, ValueType> > result;
    int count = 0;
    collectNodesDesc(root, result, count, N);
    return result;
}

// Clear helper
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::clearHelper(AVLNode<KeyType, ValueType>* node) {
    if (node == 0) return;
    
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

// Clear public method
template<class KeyType, class ValueType>
void AVLTree<KeyType, ValueType>::clear() {
    clearHelper(root);
    root = 0;
}

// Count helper
template<class KeyType, class ValueType>
int AVLTree<KeyType, ValueType>::countHelper(AVLNode<KeyType, ValueType>* node) {
    if (node == 0) return 0;
    return 1 + countHelper(node->left) + countHelper(node->right);
}

// Count public method
template<class KeyType, class ValueType>
int AVLTree<KeyType, ValueType>::count() {
    return countHelper(root);
}

// Height public method
template<class KeyType, class ValueType>
int AVLTree<KeyType, ValueType>::height() {
    return getHeight(root);
}

#endif
