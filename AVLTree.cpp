#include "AVLTree.h"
#include <string>



bool AVLTree::insert(const std::string& key, size_t value) { // insert key-value pair into AVL tree
    if (contains(key)) { // This checks if the key already exists and returns false if it does
        return false;
    }

    AVLNode* newNode = new AVLNode(key, value); // create new node with key and value

    if (!root) { // if tree is empty
        root = newNode; // new node becomes root
        return true; // insertion successful
    }

    AVLNode* current = root; // pointer starting at the root
    AVLNode* parent = nullptr; // pointer to keep track of the parent node

    while (current) {
        parent = current; // update parent to current before moving down the tree
        if (key < current->key) { // of key is less than current key, go left
            current = current->left; // move left, update current then continue
        } else { // else go right
            current = current->right; // move right, update current then continue
        }
    }
    // Now current is null and parent is the node to attach the new node to at this point
    newNode->parent = parent; // newNode's parent is the found parent
    if (key < parent->key) { // if key is less than parent's key
        parent->left = newNode; // attach new node as left child
    } else {
        parent->right = newNode; // attach new node as right child
    }
    AVLNode* needsBalanced = parent; // start balancing from the parent of the new node
    while (needsBalanced) { // will balance as long as needsBalanced is not null when null ends
        balanceNode(needsBalanced); // balance the current node
        needsBalanced = needsBalanced->parent; // Move to next parent for balancing if no parent null
    }
    return true; // insertion successful
}

bool AVLTree::remove(const std::string& key) {

}
bool AVLTree::contains(const std::string& key) const {

}
std::optional<size_t> AVLTree::get(const std::string& key) const {

}
size_t& AVLTree::operator[](const std::string& key) {

}
vector<std::string> AVLTree::findRange(const std::string& lowKey, const std::string& highKey) {

}
std::vector<std::string> AVLTree::keys() const {

}

size_t AVLTree::size() const {

}
size_t AVLTree::getHeight() const {
    if (root != nullptr) {
        return root->height;
    }
    return 0;
}
AVLTree::AVLTree(const AVLTree& other) {

}
void AVLTree::operator=(const AVLTree& other) {

}
AVLTree::~AVLTree() {

}


size_t AVLTree::AVLNode::numChildren() const {
    return 0;
}

bool AVLTree::AVLNode::isLeaf() const {
    return false;
}

size_t AVLTree::AVLNode::getHeight() const {
    return 0;
}

bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
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
        // get smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode* smallestInRight = current->right;
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

    return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    return false;
}

void AVLTree::balanceNode(AVLNode *&node) {

}
