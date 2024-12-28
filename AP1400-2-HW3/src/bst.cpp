#include "bst.h"
#include <queue>
BST::Node::Node(int value, BST::Node *left, BST::Node *right)
{
    this->value = value;
    this->left = left;
    this->right = right;
}
BST::Node::Node()
{
    this->value = 0;
    this->left = nullptr;
    this->right = nullptr;
}
BST::Node::Node(const BST::Node &node)
{
    this->value = node.value;
    this->left = node.left;
    this->right = node.right;
}
BST::Node::Node(int value)
{
    this->value=value;
    this->left = nullptr;
    this->right = nullptr;
}
std::ostream& operator<<(std::ostream& os, const BST& bst)
{
    return os << bst.root;
}
std::ostream &operator<<(std::ostream &os, const BST::Node &node)
{
    os
        << "address: " << &node << std::string(5, ' ') // 输出当前节点的地址
        << "value: " << node.value << "     ==>       "
        << "left: " << (node.left ? "0x" + std::to_string(reinterpret_cast<std::uintptr_t>(node.left)) : "nullptr") << "     ==>      "
        << "right: " << (node.right ? "0x" + std::to_string(reinterpret_cast<std::uintptr_t>(node.right)) : "nullptr");
    return os;
}
BST::BST()
{
    this->root = nullptr;
    this->size = 0;
}
BST::Node*& BST::get_root()
{
    return root;
}
void BST::bfs(std::function<void(BST::Node *&node)> func)
{
    if (this->root == nullptr)
    {
        return;
    }
    std::queue<BST::Node *> q;
    q.push(this->root);
    while (!q.empty())
    {
        BST::Node *node = q.front();
        q.pop();
        func(node);
        if (node->left != nullptr)
        {
            q.push(node->left);
        }
        if (node->right != nullptr)
        {
            q.push(node->right);
        }
    }
}
size_t BST::length()
{
    return size;
}
bool BST::add_node(int value)
{
    size++;
    if (this->root == nullptr)
    {
        this->root = new BST::Node(value, nullptr, nullptr);
        return true;
    }
    else
    {
        BST::Node *node = this->root;
        while (node != nullptr)
        {
            if (node->value == value)
            {
                size--;
                return false;
            }
            if (node->value > value)
            {
                if (node->left == nullptr)
                {
                    node->left = new BST::Node(value, nullptr, nullptr);
                    return true;
                }
                else
                {
                    node = node->left;
                }
            }
            else
            {
                if (node->right == nullptr)
                {
                    node->right = new BST::Node(value, nullptr, nullptr);
                    return true;
                }
                else
                {
                    node = node->right;
                }
            }
        }
    }
    return false;
}
BST::Node **BST::find_node(int value)
{
    if (this->root == nullptr)
    {
        return nullptr;
    }
    BST::Node **current = &this->root;
    while (*current != nullptr)
    {
        if ((*current)->value == value)
        {
            return current; // 找到目标节点
        }
        if ((*current)->value > value)
        {
            current = &((*current)->left); // 追踪左子树
        }
        else
        {
            current = &((*current)->right); // 追踪右子树
        }
    }
    return nullptr;
}
BST::Node **BST::find_parrent(int value)
{
    if (this->root == nullptr)
    {
        return nullptr;
    }
    BST::Node **root = &this->root;
    while (*root != nullptr)
    {
        if ((*root)->left != nullptr && (*root)->left->value == value)
        {
            return root;
        }
        if ((*root)->right != nullptr && (*root)->right->value == value)
        {
            return root;
        }
        if ((*root)->value > value)
        {
            root = &(*root)->left;
        }
        else
        {
            root = &(*root)->right;
        }
    }
    return nullptr;
}
BST::Node **BST::find_successor(int value)
{
    if (this->root == nullptr)
    {
        return nullptr;
    }
    BST::Node **root = &this->root;
    BST::Node **successor = nullptr;
    while (*root != nullptr)
    {
        if ((*root)->value > value)
        {
            successor = root;
            root = &(*root)->left;
        }
        else if((*root)->value<value)
        {
            root = &(*root)->right;
        }
        else  if((*root)->value == value)
        {
            if ((*root)->left != nullptr)
            {
                successor = &(*root)->left;
                while ((*successor)->right != nullptr)
                {
                    successor = &(*successor)->right;
                }
                return successor;
                break;
            }
        }
    }
    return successor;
}
void operatorpulsHlper(BST::Node* root,int value){
    if (root == nullptr)
    {
        return;
    }
    root->value++;
    operatorpulsHlper(root->left, value);
    operatorpulsHlper(root->right, value);

}
BST &BST::operator++()
{
    operatorpulsHlper(root, 1); // 调用递归函数
    return *this;
}

BST BST::operator++(int)
{
    BST temp;
    copy_tree(temp.root,this->root );// 创建一个副本
    temp.size=this->size;
    operatorpulsHlper(root, 1); // 调用递归函数
    return temp;
}

void BST::copy_tree(Node *&dest, Node *src)
{
    if (src == nullptr)
    {
        dest = nullptr;
    }
    else
    {
        dest = new Node(src->value);
        copy_tree(dest->left, src->left);
        copy_tree(dest->right, src->right);
    }
}


bool BST::delete_nodeHelper(int value)
{
    if (root == nullptr)
    {
        return false; // Tree is empty
    }

    // Search for the node to delete
    Node **node = &root;
    while (*node != nullptr)
    {
        if ((*node)->value == value)
        {
            break;
        }
        if ((*node)->value > value)
        {
            node = &(*node)->left;
        }
        else
        {
            node = &(*node)->right;
        }
    }

    if (*node == nullptr)
    {
        return false; // Node not found
    }

    // Case 1: Node has no children (leaf node)
    if ((*node)->left == nullptr && (*node)->right == nullptr)
    {
        delete *node;
        *node = nullptr; // Set pointer to nullptr after deletion
    }
    // Case 2: Node has one child (either left or right)
    else if ((*node)->left == nullptr)
    {
        Node *temp = (*node)->right;
        delete *node;
        *node = temp; // Replace with right child
    }
    else if ((*node)->right == nullptr)
    {
        Node *temp = (*node)->left;
        delete *node;
        *node = temp; // Replace with left child
    }
    // Case 3: Node has two children
    else
    {
        // Find the in-order successor (leftmost node in the right subtree)
        Node **successor = &(*node)->left;
        while ((*successor)->right != nullptr)
        {
            successor = &(*successor)->right;
        }

        // Replace the value of the node to be deleted with the successor's value
        (*node)->value = (*successor)->value;

        // Now delete the successor node, which will have at most one child
        Node *temp = *successor;
        *successor = (*successor)->left; // Move successor's right child up
        delete temp;
    }

    return true;
}

bool BST::delete_node(int value)
{
    if (delete_nodeHelper(value))
    {
        size--; // Decrement the size of the tree
        return true;
    }
    else
    {
        return false;
    }
}
