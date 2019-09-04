//
// Created by Yogesh Kumar on 2019-09-04.
//

#ifndef AVLTREE_H
#define AVLTREE_H


#include <iostream>
#include <vector>
using namespace std;

template<class Key, class Value>
struct AVLTreeNode{
    Key key;
    Value value;
    int height;
    AVLTreeNode<Key, Value>* leftChild;
    AVLTreeNode<Key, Value>* rightChild;
};

enum AVLTreeAccessBehaviorType{
    Create_If_Not_Found,
    Error_If_Not_Found,
    Default_Behavior
};

template<class Key, class Value>
class AVLTree{
private:
    // Class Members
    typedef int (*CompFuncType)(const Key&, const Key&);
    typedef AVLTreeNode<Key, Value> AVLNode;

    AVLTreeNode<Key, Value>* root;
    CompFuncType compFunc;
    AVLTreeAccessBehaviorType accessBehaviour;
    Value defaultValue;

public:
    // Constructors
    AVLTree();
    // AVLTree(vector<Key>, vector<Value>);
    // AVLTree(Key keys[], Value values[], int);
    // AVLTree(vector<pair<Key, Value>>);
    // AVLTree(pair<Key, Value> keyValuePair[], int);
    // AVLTree(const AVLTree<Key, Value>&);
    // AVLTree(const AVLTree<Key, Value>&&);

    // Destructors
    ~AVLTree();
    void recursiveDelete(AVLNode*);

    // Main Functions
    void insertNode(Key, Value);
    AVLNode* searchNode(Key);
    AVLNode* deleteNode(Key);

    // Setters
    void setAccessBehaviour(AVLTreeAccessBehaviorType type);
    void setDefaultValue(Value value);

    // Operator overloading
    AVLNode& operator[](Key);

private:
    // Helper Functions
    AVLNode* createNode(Key, Value);
    AVLNode* recursiveInsert(AVLNode*, Key&, Value&);
    AVLNode* findParentNode(Key, bool&);
    int findBalanceFactor(AVLNode*);
    void updateHeight(AVLNode*);

    // Rotations
    AVLNode* LLRotation(AVLTreeNode<Key, Value>*);
    AVLNode* LRRotation(AVLTreeNode<Key, Value>*);
    AVLNode* RLRotation(AVLTreeNode<Key, Value>*);
    AVLNode* RRRotation(AVLTreeNode<Key, Value>*);
};

#include "AVLTree.cpp"

#endif
