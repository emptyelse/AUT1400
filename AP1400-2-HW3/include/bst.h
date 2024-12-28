#ifndef BST_H
#define BST_H
#include <functional>
#include <iostream>
#include <iomanip> 
class BST
{
public:
    class Node
    {
    public:
        Node(int value, Node* left, Node* right);
        Node();
        Node(const Node& node);
        Node(int value);
        int value;
        Node* left;
        Node* right;
        bool operator==(const int value) const {
            return this->value == value;
        }
        bool operator!=(const int value) const {
            return this->value != value;
        }
        bool operator<(const int value) const {
            return this->value < value;
        }
        bool operator>(const int value) const {
            return this->value > value;
        }
        bool operator<=(const int value) const {
            return this->value <= value;
        }
        bool operator>=(const int value) const {
            return this->value >= value;
        }
    };
    BST();
    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func);
    size_t length();
    bool add_node(int value);
    Node** find_node(int value);
    Node** find_parrent(int value);
    Node** find_successor(int value);
    bool delete_node(int value);
    friend std::ostream& operator<<(std::ostream& os, const BST& bst);
    BST(std::initializer_list<int> values) : root(nullptr), size(0)
    {
        for (int value : values)
        {
            add_node(value);
        }
    }
    BST& operator++();
    BST operator++(int); // 后置++

private:
    void copy_tree(Node *&dest, Node *src);
    Node* root;
    size_t size;
    bool delete_nodeHelper(int value);
};
std::ostream& operator<<(std::ostream& os, const BST::Node& node);
// 重载 int 与 Node 的比较运算符
inline bool operator<(int val, const BST::Node& node) {
    return val < node.value;
}

inline bool operator<=(int val, const BST::Node& node) {
    return val <= node.value;
}

inline bool operator>(int val, const BST::Node& node) {
    return val > node.value;
}

inline bool operator>=(int val, const BST::Node& node) {
    return val >= node.value;
}

inline bool operator==(int val, const BST::Node& node) {
    return val == node.value;
}

inline bool operator!=(int val, const BST::Node& node) {
    return val != node.value;
}

#endif //BST_H