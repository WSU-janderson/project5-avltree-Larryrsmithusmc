#include "AVLTree.h"
#include <string>


bool AVLTree::insert(const std::string &key, size_t value) {
    // insert key-value pair into AVL tree
    if (contains(key)) {
        // This checks if the key already exists and returns false if it does
        return false;
    }
    AVLNode* newNode = new AVLNode(key, value); // create new node with key and value
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
            insertRecursive(parent->left, nodeToInsert); // parent is now left child, recursive call
        }
    } else {
        if (parent->right == nullptr) {
            // if right child is null
            parent->right = nodeToInsert; // insert as right child
            nodeToInsert->parent = parent; // set parent of inserted node for balancing
        } else {
            insertRecursive(parent->right, nodeToInsert); // parent is now right child, recursive call
        }
    }
    // After insertion, update height and balance the tree
    parent->height = parent->getHeight(); // update height of parent
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
    return treeSize;
}

size_t AVLTree::getHeight(){
    return getHeightHelper(root);
}

AVLTree::AVLTree(const AVLTree &other) {
    root = copyTree(other.root); // copy the tree from other tree and stores return pointer in root
    treeSize = other.treeSize; // copy size from other tree
}

void AVLTree::operator=(const AVLTree &other) {
}

AVLTree::~AVLTree() {
    deleteTree(root);
}
AVLTree::AVLTree() : root(), treeSize(0) {
    // constructor initializes root to null and size to 0
}
size_t AVLTree::AVLNode::numChildren() const {
}

bool AVLTree::AVLNode::isLeaf() const {
    return left == nullptr && right == nullptr; // A node is a leaf if left and right children are null
}

size_t AVLTree::AVLNode::getHeight() {
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
void AVLTree::deleteTree(AVLNode* current) {
    if (!current) {
        return; // base case: current is null
    }
    deleteTree(current->left); // recursive call to delete left subtree
    deleteTree(current->right); // recursive call to delete right subtree
    delete current; // delete current node
}
size_t AVLTree::getHeightHelper(AVLNode* current) {
    if (!current) {
        return 0; // base case: current is null returns
    }
    size_t leftHeight = getHeightHelper(current->left); // get height of left subtree
    size_t rightHeight = getHeightHelper(current->right); // get height of right subtree
    // size_t cannot be a negative number so need to check if leaf node and assign it with 0 or else a leaf node will return as 1
    if (current->isLeaf()) {
        return 0; // leaf node has height 0
    }
    return 1 + max(leftHeight, rightHeight); // return max height plus one for current node
}

AVLTree::AVLNode* AVLTree::copyTree(const AVLNode *current) {
    if (!current) {
        return nullptr; // base case: current is null
    }
    AVLNode* newNode = new AVLNode(current->key, current->value); // create new node with current key and value
    newNode->height = current->height; // copy height
    newNode->balance = current->balance; // copy balance

    newNode->left = copyTree(current->left);
    newNode->right = copyTree(current->right);

    return newNode;
}
