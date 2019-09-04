//
// Created by Yogesh Kumar on 2019-09-04.
//

#include "AVLTree.h"
#include <vector>

int max(int x, int y){
    if(x > y) return x;
    return y;
}

// ----------------- CONSTRUCTOR ----------------------
template<class Key, class Value>
AVLTree<Key, Value>::AVLTree(){
    root = nullptr;
    cacheAvailable = false;
    cachedNode = nullptr;
    compFunc = [](const Key& a, const Key& b)->int{
        if(a < b) return -1;
        if(a > b) return 1;
        return 0;
    };
}

// ------------------ DESTRUCTOR ----------------------
template<class Key, class Value>
AVLTree<Key, Value>::~AVLTree(){
    recursiveDelete(root);
}

template<class Key, class Value>
void AVLTree<Key, Value>::recursiveDelete(AVLTreeNode<Key, Value>* current){
    if(!current) return;
    recursiveDelete(current->leftChild);
    recursiveDelete(current->rightChild);
    delete current;
}

// -------------------- INSERT ------------------------
template<class Key, class Value>
bool AVLTree<Key, Value>::insert(Key key, Value value){
    operationResult = true;
    root = recursiveInsert(root, key, value);
    return operationResult;
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::recursiveInsert(AVLNode* current, Key& key, Value& value){
    if(!current)  return cachedNode = createNode(key, value);

    switch(compFunc(current->key, key)){
        case 0  : 
            cacheAvailable = true;    
            operationResult = false;
            return cachedNode = current;
        case 1  : current->leftChild = recursiveInsert(current->leftChild, key, value);   break;
        case -1 : current->rightChild = recursiveInsert(current->rightChild, key, value); break;
    }

    int balanceFactor = findBalanceFactor(current);
    if(balanceFactor > 1){
        // Left Imbalance
        int childBalanceFactor = findBalanceFactor(current->leftChild);
        if(childBalanceFactor > 0) current = LLRotation(current);
        if(childBalanceFactor < 0) current = LRRotation(current);
        updateHeight(current->leftChild);
        updateHeight(current->rightChild);
    }
    else if(balanceFactor < -1){
        // Right Imbalance
        int childBalanceFactor = findBalanceFactor(current->rightChild);
        if(childBalanceFactor > 0) current = RLRotation(current);
        if(childBalanceFactor < 0) current = RRRotation(current);
        updateHeight(current->leftChild);
        updateHeight(current->rightChild);
    }
    updateHeight(current);
    return current;
}

template<class Key, class Value>
int AVLTree<Key, Value>::findBalanceFactor(AVLNode* node){
    int leftHeight = 0, rightHeight = 0;
    if(node->leftChild) leftHeight = node->leftChild->height;
    if(node->rightChild) rightHeight = node->rightChild->height;
    return (leftHeight - rightHeight);
}

template<class Key, class Value>
void AVLTree<Key, Value>::updateHeight(AVLNode* node){
    if(!node) return;
    int leftHeight = 0, rightHeight = 0;
    if(node->leftChild) leftHeight = node->leftChild->height;
    if(node->rightChild) rightHeight = node->rightChild->height;
    node->height = max(leftHeight, rightHeight) + 1;
}

// -------------------- SEARCH ------------------------
template<class Key, class Value>
Value& AVLTree<Key, Value>::search(Key& key){
    if(cacheAvailable && cachedNode->key == key) return cachedNode->value;
    AVLNode* foundNode = searchNode(key);
    if(!foundNode){
        // Throw error
    }
    return foundNode->value;
}

template<class Key, class Value>
bool AVLTree<Key, Value>::has(Key& key){
    if(cacheAvailable && cachedNode->key == key) return true;
    AVLNode* foundNode = searchNode(key);
    return (foundNode == nullptr);
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::searchNode(Key key){
    AVLTreeNode<Key, Value>* current = root;
    AVLTreeNode<Key, Value> foundNode;
    while(current){
        switch(compFunc(current->key, key)){
            case 0  :   cacheAvailable = true;              return cachedNode = current;
            case 1  :   current = current->leftChild;       break;
            case -1 :   current = current->rightChild;      break;
        }
    }

    // Node is not present;
    return nullptr;
}

// -------------------- DELETE ------------------------
template<class Key, class Value>
bool AVLTree<Key, Value>::remove(Key& key){
    if(cacheAvailable && cachedNode->key == key){
        cacheAvailable = false;
        cachedNode = nullptr;
    }

    operationResult = true;
    root = recursiveRemove(root, key);
    return operationResult;
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::recursiveRemove(AVLTreeNode<Key, Value>* current, Key& key){
    if(!current){
        operationResult = false;
        return nullptr;
    }

    switch(compFunc(current->key, key)){
        case 0  : 
            // CASE : Both Child
            if(current->leftChild && current->rightChild){
                AVLNode* minNode = findMinNode(current->rightChild);
                current->rightChild = recursiveRemove(current->rightChild, minNode->key);
                minNode->rightChild = current->rightChild;
                minNode->leftChild = current->leftChild;
                updateHeight(minNode);
                delete current;
                return minNode;
            }

            // CASE : Exactly One Child
            if(current->leftChild) return current->leftChild;
            else return current->rightChild;

        case 1  : current->leftChild = recursiveRemove(current->leftChild, key);   break;
        case -1 : current->rightChild = recursiveRemove(current->rightChild, key); break;
    }

    int balanceFactor = findBalanceFactor(current);
    if(balanceFactor > 1){
        // Left Imbalance
        int childBalanceFactor = findBalanceFactor(current->leftChild);
        if(childBalanceFactor > 0) current = LLRotation(current);
        if(childBalanceFactor < 0) current = LRRotation(current);
        updateHeight(current->leftChild);
        updateHeight(current->rightChild);
    }
    else if(balanceFactor < -1){
        // Right Imbalance
        int childBalanceFactor = findBalanceFactor(current->rightChild);
        if(childBalanceFactor > 0) current = RLRotation(current);
        if(childBalanceFactor < 0) current = RRRotation(current);
        updateHeight(current->leftChild);
        updateHeight(current->rightChild);
    }
    updateHeight(current);
    return current;
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::replaceWithMin(AVLTreeNode<Key, Value>* current, AVLTreeNode<Key, Value>* parent){
    if(!current->leftChild){
        parent->leftChild = current->rightChild;
        current->rightChild = nullptr;
        return current;
    }

    AVLNode* minNode = replaceWithMin(current->leftChild, current);
    updateHeight(current);
    return minNode;
} 

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::findParentNode(Key key, bool& nodeExists){
    AVLNode* current = root, parent = nullptr;

    while(current){
        parent = current;
        switch(compFunc(current->key, key)){
            case 0  :   nodeExists = true;                  return current;
            case 1  :   current = current->leftChild;       break;
            case -1 :   current = current->rightChild;      break;
        }
    }

    // Node is not present;
    nodeExists = false;
    return parent;
}

// -------------- ACCESSORY FUNCTIONS -----------------
template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::findMinNode(AVLNode* current){
    while(current->leftChild){
        current = current->leftChild;
    }
    return current;
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::findMaxNode(AVLNode* current){
    while(current->rightChild){
        current = current->rightChild;
    }
    return current;
}

// -------------------- SETTERS -----------------------
template<class Key, class Value>
void AVLTree<Key, Value>::setAccessBehaviour(AVLTreeAccessBehaviorType type){
    accessBehaviour = type;
}

template<class Key, class Value>
void AVLTree<Key, Value>::setDefaultValue(Value value){
    defaultValue = value;
}

// ------------------ CREATE NODE ---------------------
template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::createNode(Key key, Value value){
    AVLNode* node = new AVLNode;
    node->key = key;
    node->value = value;
    node->height = 1;
    node->leftChild = nullptr;
    node->rightChild = nullptr;
    return node;
}


// -------------------- ROTATIONS ---------------------
template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::LLRotation(AVLTreeNode<Key, Value>* pivot){
    AVLNode* transferedNode = pivot->leftChild->rightChild;
    AVLNode* newPivot = pivot->leftChild;
    newPivot->rightChild = pivot;
    pivot->leftChild = transferedNode;
    return newPivot;
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::LRRotation(AVLTreeNode<Key, Value>* pivot){
    AVLNode* newPivot = pivot->leftChild->rightChild;
    AVLNode* transferedNodeLeft = newPivot->leftChild;
    AVLNode* transferedNodeRight = newPivot->rightChild;
    newPivot->rightChild = pivot;
    newPivot->leftChild = pivot->leftChild;
    pivot->leftChild->rightChild = transferedNodeLeft;
    pivot->leftChild = transferedNodeRight;
    return newPivot;
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::RLRotation(AVLTreeNode<Key, Value>* pivot){
    AVLNode* newPivot = pivot->rightChild->leftChild;
    AVLNode* transferedNodeLeft = newPivot->leftChild;
    AVLNode* transferedNodeRight = newPivot->rightChild;
    newPivot->leftChild = pivot;
    newPivot->rightChild = pivot->rightChild;
    pivot->rightChild->leftChild = transferedNodeRight;
    pivot->rightChild = transferedNodeLeft;
    return newPivot;
}

template<class Key, class Value>
AVLTreeNode<Key, Value>* AVLTree<Key, Value>::RRRotation(AVLTreeNode<Key, Value>* pivot){
    AVLNode* transferedNode = pivot->rightChild->leftChild;
    AVLNode* newPivot = pivot->rightChild;
    newPivot->leftChild = pivot;
    pivot->rightChild = transferedNode;
    return newPivot;
}

// --------------- OPERATOR OVERLOADING -----------------
template<class Key, class Value>
Value& AVLTree<Key, Value>:: operator[](Key key){
    if(cacheAvailable && cachedNode->key == key) return cachedNode->value;
    AVLNode* foundNode;
    switch(accessBehaviour){
        case Default_Behavior :
        case Error_If_Not_Found:
            foundNode = searchNode(key);
            if(!foundNode){
                // Throw error
                // throw
            }
            return foundNode->value;

            break;
        case Create_If_Not_Found:
            insert(key, defaultValue);
            return cachedNode->value;
    }
}

