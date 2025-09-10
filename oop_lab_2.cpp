#include <iostream>

using color = enum {red, black};

class node {
private:
    int key = 0;
    color cl = black;
    node* left = nullptr;
    node* right = nullptr;
    node* parent = nullptr;
public:
    node() {}
    node(int v) {key = v;}
    node(int v, color cl) {key = v; this->cl = cl;}
    node(int v, color cl, node* p) {key = v; this->cl = cl; parent = p;}

    node* _find(int key, node* root) {
        if(root == nullptr)
            return nullptr;
        if(root->key > key)
            return _find(key, root->left);
        if(root->key < key)
            return _find(key, root->right);
        return root;
    }

    void _clear(node* root) {
        if(root == nullptr)
            return;
        if(root->left != nullptr)
            _clear(root->left);
        if(root->right != nullptr)
            _clear(root->right);
        delete root;
    }
};

class set {
private:
    node* root;
    size_t sz;
public:
    set() {root = nullptr; sz = 0;}

    node* find(int key) {return root->_find(key, root);}

    void clear() {
        root->_clear(root);
        root = nullptr;
        sz = 0;
    }

    bool contains(int key) {return root->_find(key, root);}

    bool empty() {return root == nullptr;}

    size_t size() {return sz;}

    ~set() {
        root->_clear(root);
        root = nullptr;
        sz = 0;
    }
};

int main(void) {

    return 0;
}