#include <iostream>
#include <queue>

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

    int& get_key() {return key;}

    color& get_color() {return cl;}

    node* get_parent() {return parent;}

    node* get_left_succ() {return left;}

    node* get_right_succ() {return right;}

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

    set(set& other) {

    }

    node* find(int key) {
        node* it = root;

        while(it != nullptr) {
            if(it->get_key() > key)
                it = it->get_left_succ();
            else if(it->get_key() < key)
                it = it->get_right_succ();
            else
                break;
        }

        return it;
    }

    void clear() {
        root->_clear(root);
        root = nullptr;
        sz = 0;
    }

    bool contains(int key) {return find(key);}

    bool empty() {return root == nullptr;}

    bool is_equal(set& first, set& second) {
        std::queue<node*> f_queue;
        std::queue<node*> s_queue;

        //while()
    }

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