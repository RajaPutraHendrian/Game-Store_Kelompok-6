#ifndef BST_
#define BST_

#include <iostream>
#include <string>
#include "xcept.h"
using namespace std;

// Generic BST Node
template<class KeyType, class ValueType>
class BSTNode {
public:
    KeyType key;
    ValueType value;
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(const KeyType& k, const ValueType& v)
        : key(k), value(v), left(0), right(0) {}
};

// Generic Binary Search Tree
template<class KeyType, class ValueType>
class BST {
private:
    BSTNode<KeyType, ValueType>* root;
    
    // Recursive helpers
    BSTNode<KeyType, ValueType>* insertHelper(BSTNode<KeyType, ValueType>* node, 
                                               const KeyType& key, const ValueType& value);
    BSTNode<KeyType, ValueType>* searchHelper(BSTNode<KeyType, ValueType>* node, 
                                               const KeyType& key);
    void inorderHelper(BSTNode<KeyType, ValueType>* node, 
                       void (*visit)(const KeyType&, const ValueType&));
    void clearHelper(BSTNode<KeyType, ValueType>* node);
    int countHelper(BSTNode<KeyType, ValueType>* node);
    int heightHelper(BSTNode<KeyType, ValueType>* node);
    
    // Range search helper
    void rangeSearchHelper(BSTNode<KeyType, ValueType>* node, 
                          const KeyType& min, const KeyType& max,
                          void (*visit)(const KeyType&, const ValueType&));
    
public:
    BST();
    ~BST();
    
    void insert(const KeyType& key, const ValueType& value);
    ValueType* search(const KeyType& key);
    void inorderTraversal(void (*visit)(const KeyType&, const ValueType&));
    void rangeSearch(const KeyType& min, const KeyType& max,
                    void (*visit)(const KeyType&, const ValueType&));
    
    bool isEmpty() const { return root == 0; }
    int count();
    int height();
    void clear();
};

// Constructor
template<class KeyType, class ValueType>
BST<KeyType, ValueType>::BST() : root(0) {}

// Destructor
template<class KeyType, class ValueType>
BST<KeyType, ValueType>::~BST() {
    clear();
}

// Insert helper (recursive)
template<class KeyType, class ValueType>
BSTNode<KeyType, ValueType>* BST<KeyType, ValueType>::insertHelper(
    BSTNode<KeyType, ValueType>* node, const KeyType& key, const ValueType& value) {
    
    if (node == 0) {
        return new BSTNode<KeyType, ValueType>(key, value);
    }
    
    if (key < node->key) {
        node->left = insertHelper(node->left, key, value);
    } else if (key > node->key) {
        node->right = insertHelper(node->right, key, value);
    } else {
        // Key already exists, update value
        node->value = value;
    }
    
    return node;
}

// Insert public method
template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    root = insertHelper(root, key, value);
}

// Search helper (recursive)
template<class KeyType, class ValueType>
BSTNode<KeyType, ValueType>* BST<KeyType, ValueType>::searchHelper(
    BSTNode<KeyType, ValueType>* node, const KeyType& key) {
    
    if (node == 0) {
        return 0;  // Not found
    }
    
    if (key == node->key) {
        return node;  // Found!
    } else if (key < node->key) {
        return searchHelper(node->left, key);
    } else {
        return searchHelper(node->right, key);
    }
}

// Search public method
template<class KeyType, class ValueType>
ValueType* BST<KeyType, ValueType>::search(const KeyType& key) {
    BSTNode<KeyType, ValueType>* result = searchHelper(root, key);
    if (result) {
        return &(result->value);
    }
    return 0;
}

// Inorder traversal helper (Left-Root-Right = sorted!)
template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::inorderHelper(
    BSTNode<KeyType, ValueType>* node,
    void (*visit)(const KeyType&, const ValueType&)) {
    
    if (node == 0) return;
    
    inorderHelper(node->left, visit);
    visit(node->key, node->value);
    inorderHelper(node->right, visit);
}

// Inorder traversal public method
template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::inorderTraversal(
    void (*visit)(const KeyType&, const ValueType&)) {
    inorderHelper(root, visit);
}

// Range search helper
template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::rangeSearchHelper(
    BSTNode<KeyType, ValueType>* node,
    const KeyType& min, const KeyType& max,
    void (*visit)(const KeyType&, const ValueType&)) {
    
    if (node == 0) return;
    
    // If current key > min, check left subtree
    if (node->key > min) {
        rangeSearchHelper(node->left, min, max, visit);
    }
    
    // If current key is in range, process it
    if (node->key >= min && node->key <= max) {
        visit(node->key, node->value);
    }
    
    // If current key < max, check right subtree
    if (node->key < max) {
        rangeSearchHelper(node->right, min, max, visit);
    }
}

// Range search public method
template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::rangeSearch(
    const KeyType& min, const KeyType& max,
    void (*visit)(const KeyType&, const ValueType&)) {
    rangeSearchHelper(root, min, max, visit);
}

// Clear helper
template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::clearHelper(BSTNode<KeyType, ValueType>* node) {
    if (node == 0) return;
    
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

// Clear public method
template<class KeyType, class ValueType>
void BST<KeyType, ValueType>::clear() {
    clearHelper(root);
    root = 0;
}

// Count helper
template<class KeyType, class ValueType>
int BST<KeyType, ValueType>::countHelper(BSTNode<KeyType, ValueType>* node) {
    if (node == 0) return 0;
    return 1 + countHelper(node->left) + countHelper(node->right);
}

// Count public method
template<class KeyType, class ValueType>
int BST<KeyType, ValueType>::count() {
    return countHelper(root);
}

// Height helper
template<class KeyType, class ValueType>
int BST<KeyType, ValueType>::heightHelper(BSTNode<KeyType, ValueType>* node) {
    if (node == 0) return 0;
    
    int leftHeight = heightHelper(node->left);
    int rightHeight = heightHelper(node->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Height public method
template<class KeyType, class ValueType>
int BST<KeyType, ValueType>::height() {
    return heightHelper(root);
}

#endif
