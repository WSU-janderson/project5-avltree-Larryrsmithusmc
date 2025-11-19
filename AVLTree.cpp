/*
 * Larry Smith
 * Project #5
 * CS 3100
 * Map ADT: AVL Tree
 * 11/19/2025
 */
#include "AVLTree.h"
#include <string>
#include <iostream>


bool AVLTree::insert(const std::string &key, size_t value) {
    // insert key-value pair into AVL tree
    if (contains(key)) {
        // This checks if the key already exists and returns false if it does
        return false;
    }
    AVLNode *newNode = new AVLNode(key, value); // create new node with key and value
    if (!root) {
        // if tree is empty
        root = newNode; // new node becomes root
    } else {
        insertRecursive(root, newNode); // call recursive helper to insert node starting from root
    }
    treeSize++; // increment size of tree upon successful insertion
    return true; // insertion successful
}

void AVLTree::insertRecursive(AVLNode *&parent, AVLNode *&nodeToInsert) {
    // recursive helper to insert node
    if (nodeToInsert->key < parent->key) {
        // if key to insert is less than parent's key
        if (parent->left == nullptr) {
            // if left child is null
            parent->left = nodeToInsert; // insert as left child
            nodeToInsert->parent = parent; // set parent of inserted node for balancing
        } else {
            insertRecursive(parent->left, nodeToInsert); // recursive call to left subtree
        }
    } else {
        if (parent->right == nullptr) {
            // if right child is null
            parent->right = nodeToInsert; // insert as right child
            nodeToInsert->parent = parent; // set parent of inserted node for balancing
        } else {
            insertRecursive(parent->right, nodeToInsert); // recursive call to right subtree
        }
    }
    // After insertion, update height and balance the tree
    updateHeight(parent);
    balanceNode(parent); // balance the parent node
}

bool AVLTree::remove(const std::string &key) {
    return remove(root, key); // call recursive remove starting from root to remove given key
}

bool AVLTree::contains(const std::string &key) const {
    return contains(root, key); // call recursive contains starting from root to check for key
}

std::optional<size_t> AVLTree::get(const std::string &key) const {
    return get(root, key); // call recursive get starting from root to retrieve value for key
}

size_t &AVLTree::operator[](const std::string &key) {
    // overload operator to access value by key
    return getValue(root, key);
}

vector<std::string> AVLTree::findRange(const std::string &lowKey, const std::string &highKey) {
    vector<string> keys; // vector to store keys in range
    findKeysInRange(root, lowKey, highKey, keys); // call helper to find keys in range
    return keys; // return vector of keys
}

std::vector<std::string> AVLTree::keys() const {
    vector<string> keys; // vector to store all keys
    allKeys(root, keys); // call helper to get all keys starting from root
    return keys; // return vector of all keys
}

size_t AVLTree::size() const {
    // return size of tree
    return treeSize;
}

size_t AVLTree::getHeight() {
    // return height of tree
    return getHeightHelper(root);
}

AVLTree::AVLTree(const AVLTree &other) : root(), treeSize(0) {
    // copy constructor
    root = copyTree(other.root); // copy the tree from other tree and stores return pointer in root
    treeSize = other.treeSize; // copy size from other tree
}

void AVLTree::operator=(const AVLTree &other) {
    // assignment operator
    if (this != &other) {
        // only copy if this and other are different
        deleteTree(root); // delete current tree to avoid memory leaks
        root = copyTree(other.root); // copy the tree from other tree
        treeSize = other.treeSize; // copy size from other tree
    }
}

AVLTree::~AVLTree() {
    // destructor
    deleteTree(root); // delete the tree to free memory
    root = nullptr; // set root to null after deletion
    treeSize = 0; // set size to 0 after deletion
}

std::ostream &operator<<(std::ostream &os, const AVLTree &tree) {
    tree.printTree(tree.root, os, 0); // call printTree helper to print the tree
    return os;
}

AVLTree::AVLTree() : root(), treeSize(0) {
    // constructor initializes root to null and size to 0
}

size_t AVLTree::AVLNode::numChildren() const {
    size_t numChildren = 0;
    if (left) {
        numChildren++; // increment if left child exists
    }
    if (right) {
        numChildren++; // increment if right child exists
    }
    return numChildren; // return total number of children
}

bool AVLTree::AVLNode::isLeaf() const {
    return left == nullptr && right == nullptr; // A node is a leaf if left and right children are null
}

size_t AVLTree::AVLNode::getHeight() {
    // return height
    return getHeightHelper(this);
}

bool AVLTree::removeNode(AVLNode *&current) {
    if (!current) {
        return false;
    }

    AVLNode *toDelete = current;
    auto nChildren = current->numChildren();
    if (current->isLeaf()) {
        // case 1 we can delete the node
        current = nullptr;
    } else if (current->numChildren() == 1) {
        // case 2 - replace current with its only child
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        // case 3 - we have two children,
        // get the smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode *smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;

        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;
    treeSize--; // decrement tree size after successful deletion
    return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    if (!current) {
        // base case: current is null
        return false; // key not found
    }
    if (key < current->key) {
        // if key is less than current key
        return remove(current->left, key); // changes current to left child, recursive call
    } else if (key > current->key) {
        // if key is greater than current key
        return remove(current->right, key); // changes current to right child, recursive call
    } else {
        return removeNode(current); // key found, remove node
    }
}

void AVLTree::balanceNode(AVLNode *&node) {
    updateHeight(node); // update height of node
    if (getBalance(node) == -2) {
        if (getBalance(node->right) == 1) {
            // double rotation case
            rotateRight(node->right);
        }
        rotateLeft(node); // single rotation case
    } else if (getBalance(node) == 2) {
        if (getBalance(node->left) == -1) {
            // double rotation case
            rotateLeft(node->left);
        }
        rotateRight(node); // single rotation case
    }
}

bool AVLTree::contains(AVLNode *current, KeyType key) const {
    if (!current) {
        // base case: current is null
        return false; // key not found
    }
    if (key == current->key) {
        // if key matches current key
        return true; // key found
    }
    if (key < current->key) {
        // if key is less than current key
        return contains(current->left, key); // go left, recursive call
    } else {
        return contains(current->right, key); // go right, recursive call
    }
}

std::optional<size_t> AVLTree::get(AVLNode *current, KeyType key) const {
    if (!current) {
        // base case: current is null
        return std::nullopt; // key not found
    }
    if (key == current->key) {
        // if key matches current key
        return current->value; // return value
    }
    if (key < current->key) {
        // if key is less than current key
        return get(current->left, key); // go left, recursive call
    } else {
        return get(current->right, key); // go right, recursive call
    }
}

size_t &AVLTree::getValue(AVLNode *&current, KeyType key) {
    if (key == current->key) {
        // if key matches current key
        return current->value; // return reference to value
    }
    if (key < current->key) {
        // if key is less than current key
        return getValue(current->left, key); // go left, recursive call
    } else {
        return getValue(current->right, key); // go right, recursive call
    }
}

void AVLTree::findKeysInRange(AVLNode *current, const std::string &lowKey, const std::string &highKey,
                              vector<string> &keys) {
    if (!current) {
        // base case: current is null
        return;
    }
    if (current->key > lowKey) {
        // if current key is greater than lowKey
        findKeysInRange(current->left, lowKey, highKey, keys); // go left, recursive call
    }
    if (current->key >= lowKey && current->key <= highKey) {
        // if current key is within range
        keys.push_back(current->key); // add key to vector
    }
    if (current->key < highKey) {
        // if current key is less than highKey
        findKeysInRange(current->right, lowKey, highKey, keys); // go right, recursive call
    }
}

void AVLTree::allKeys(AVLNode *current, vector<string> &keys) const {
    if (!current) {
        // base case: current is null
        return;
    }
    keys.push_back(current->key); // add current key to vector each time we visit a node
    allKeys(current->left, keys); // go left, recursive call
    allKeys(current->right, keys); // go right, recursive call
}

void AVLTree::deleteTree(AVLNode *current) {
    if (!current) {
        return; // base case: current is null
    }
    deleteTree(current->left); // recursive call to delete left subtree
    deleteTree(current->right); // recursive call to delete right subtree
    delete current; // delete current node
}

size_t AVLTree::getHeightHelper(AVLNode *current) {
    if (!current) {
        return 0; // base case: current is null returns
    }
    size_t leftHeight = getHeightHelper(current->left); // get height of left subtree
    size_t rightHeight = getHeightHelper(current->right); // get height of right subtree
    if (current->isLeaf()) {
        return 0; // return 0 for leaf nodes
    }
    return 1 + max(leftHeight, rightHeight); // return max height plus one for current node
}

AVLTree::AVLNode *AVLTree::copyTree(const AVLNode *current) {
    if (!current) {
        return nullptr; // base case: current is null
    }
    AVLNode *newNode = new AVLNode(current->key, current->value); // create new node with current key and value
    newNode->height = current->height; // copy height
    newNode->balance = current->balance; // copy balance

    newNode->left = copyTree(current->left); // recursively copy left subtree
    newNode->right = copyTree(current->right); // recursively copy right subtree

    return newNode;
}

void AVLTree::printTree(AVLNode *current, std::ostream &os, int depth) const {
    if (!current) {
        return;
    }
    printTree(current->right, os, depth + 1); // print right subtree first
    for (int i = 0; i < depth; i++) {
        // depth indicates level in tree and helps with indentation
        std::cout << "    "; // Indentation to help it look more like a tree
    }
    std::cout << "{ " << current->key << ", " << current->value << " }" << std::endl;
    printTree(current->left, os, depth + 1); // print left subtree
}

bool AVLTree::setChild(AVLNode *parent, const std::string &whichChild, AVLNode *child) {
    // set child of parent
    if (whichChild != "left" && whichChild != "right") {
        return false; // not a valid child
    }
    if (whichChild == "left") {
        // set as left child of parent and update pointer
        parent->left = child;
    } else {
        // set as right child of parent and update pointer
        parent->right = child;
    }
    if (child != nullptr) {
        // set parent pointer of child if child is not null
        child->parent = parent;
    }
    return true;
}

bool AVLTree::replaceChild(AVLNode *parent, AVLNode *currentChild, AVLNode *newChild) {
    if (parent->left == currentChild) {
        // replace left child
        return setChild(parent, "left", newChild);
    } else if (parent->right == currentChild) {
        // replace right child
        return setChild(parent, "right", newChild);
    }
    return false;
}

void AVLTree::updateHeight(AVLNode *current) {
    // update height of current node
    if (!current) {
        return;
    }
    int leftHeight = -1; // initialize left height
    if (current->left != nullptr) {
        // if left child exists
        leftHeight = current->left->height; // set left height
    }
    int rightHeight = -1; // initialize right height
    if (current->right != nullptr) {
        // if right child exists
        rightHeight = current->right->height; // set right height
    }
    current->height = 1 + std::max(leftHeight, rightHeight); // update height of current node
}

int AVLTree::getBalance(AVLNode *current) {
    // get balance factor of current node
    int leftHeight = -1; // initialize left height
    if (current->left != nullptr) {
        // if left child exists
        leftHeight = current->left->height; // set left height
    }
    int rightHeight = -1; // initialize right height
    if (current->right != nullptr) {
        // if right child exists
        rightHeight = current->right->height; // set right height
    }
    return leftHeight - rightHeight; // return balance factor
}

AVLTree::AVLNode *AVLTree::rotateRight(AVLNode *current) {
    AVLNode *temp = current->left->right;
    if (current->parent != nullptr) {
        replaceChild(current->parent, current, current->left);
    } else {
        // current is root
        root = current->left;
        root->parent = nullptr;
    }
    setChild(current->left, "right", current); // set current as right child of its left child
    setChild(current, "left", temp); // attach temp as left child of current

    updateHeight(current);
    updateHeight(current->parent);

    return current->parent; // return new root of rotated subtree
}

AVLTree::AVLNode *AVLTree::rotateLeft(AVLNode *current) {
    AVLNode *temp = current->right->left;
    if (current->parent != nullptr) {
        replaceChild(current->parent, current, current->right);
    } else {
        // current is root
        root = current->right;
        root->parent = nullptr;
    }
    setChild(current->right, "left", current); // set current as left child of its right child
    setChild(current, "right", temp); // attach temp as right child of current

    updateHeight(current); // update height of current
    updateHeight(current->parent); // update height of parent

    return current->parent; // return new root of rotated subtree
}
