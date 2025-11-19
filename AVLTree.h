/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <optional>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;

    AVLTree();
    bool insert(const std::string& key, size_t value);
    bool remove(const std::string& key);
    bool contains(const std::string& key) const;
    std::optional<size_t> get(const std::string& key) const;
    size_t& operator[](const std::string& key);
    vector<std::string> findRange(const std::string& lowKey, const std::string& highKey);
    std::vector<std::string> keys() const;
    size_t size() const;
    size_t getHeight();
    AVLTree(const AVLTree& other);
    ~AVLTree();
    void operator=(const AVLTree& other);


    friend std::ostream& operator<<(ostream& os, const AVLTree & avlTree);

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        int balance;


        AVLNode* left;
        AVLNode* right;
        AVLNode* parent;

        AVLNode(KeyType  key, ValueType value) : key(std::move(key)), value(value), height(1), balance(0), left(nullptr), right(nullptr), parent(nullptr) {
        }

        // 0, 1 or 2
        size_t numChildren() const;
        // true or false
        bool isLeaf() const;

        size_t getHeight();

    };

public:




    private:
    AVLNode* root;
    size_t treeSize;

    /* Helper methods for remove */
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);
    // removeNode contains the logic for actually removing a node based on the number of children
    bool removeNode(AVLNode*& current);
    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);

    bool contains(AVLNode* current, KeyType key) const;

    void insertRecursive(AVLNode*& parent, AVLNode*& nodeToInsert);

    std::optional<size_t> get(AVLNode* current, KeyType Key) const;

    size_t& getValue(AVLNode*& current, KeyType key);

    void findKeysInRange(AVLNode* current, const std::string& lowKey, const std::string& highKey, vector<string>& keys);

    void allKeys(AVLNode* current, vector<string>& keys) const;

    void deleteTree(AVLNode* current);

    static size_t getHeightHelper(AVLNode* current);

    AVLNode* copyTree(const AVLNode *current);
};

#endif //AVLTREE_H
