#include <iostream>
#include <vector>

using color = enum {red, black};

class node {
private:
    int key = 0;
    color cl = black;
    node* left = nullptr;
    node* right = nullptr;
    node* parent = nullptr;

    //node* rotate_left()

    //node* rotate_right()
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

    node* _at(node* root, int pos, int& cur_pos) {
        if(root->left != nullptr)
            return _at(root->left, pos, cur_pos);
        if(cur_pos == pos)
            return root;
        cur_pos += 1;
        if(root->right != nullptr)
            return _at(root->right, pos, cur_pos);
    }

    bool _is_equal(node* ft, node* sc) {
        if(!ft && sc || ft && !sc)
            return false;
        if(ft == nullptr && sc == nullptr)
            return true;
        if(ft->key != sc->key)
            return false;
        return _is_equal(ft->left, sc->left) && _is_equal(ft->right, sc->right);
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

    set(size_t size) {
        //for(size_t i = 0; i < size; i++) 
            //insert(i);
    }

    set(int* array, size_t size) {
        //for(size_t i = 0; i < size; i++)
            //insert(array[i]);
    }

    set(std::vector<int>::iterator first, std::vector<int>::iterator last) {
        //for(auto it = first; it != last; it++) 
            //insert(*it);
    }

    set(set& other) {

    }

    node* find(int key) {return root->_find(key, root);}

    node* at(int pos) { 
        if(pos < 0)
            pos = sz + pos;
        if(pos >= sz)
            return nullptr;
        int cur_pos = 0;
        return root->_at(root, pos, cur_pos);
    }

    void clear() {
        root->_clear(root);
        root = nullptr;
        sz = 0;
    }

    bool contains(int key) {return root->_find(key, root) != nullptr;}

    bool empty() {return root == nullptr;}

    bool is_equal(set& first, set& second) {return root->_is_equal(first.root, second.root);}

    //void assign() {}

    //void resize() {}

    //void swap() {}

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