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
public:
    node() {}

    node(int v) {key = v;}

    node(int v, color cl) {key = v; this->cl = cl;}

    node(int v, color cl, node* p) {key = v; this->cl = cl; parent = p;}

    int get_key() {return key;}

    color get_color() {return cl;}

    node* get_parent() {return parent;}

    node* get_left() {return left;}

    node* get_right() {return right;}
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

    set(const int array[], size_t size) {
        //for(size_t i = 0; i < size; i++)
            //insert(array[i]);
    }

    set(set& other) {

    }

    ~set() {clear();}

    bool empty() {return sz == 0;}

    size_t size() {return sz;}

    void clear() {
        if(root == nullptr)
            return;

        std::queue<node*> q;
        q.push(root);
        while(q.empty() == false) {
            if(q.front()->get_left() != nullptr) 
                q.push(q.front()->get_left());
            if(q.front()->get_right() != nullptr) 
                q.push(q.front()->get_right());
            delete q.front();
            q.pop();
        }

        root = nullptr;
    }

    void insert(int key) {

    }

    void erase(int key) {

    }

    node* find(int key) {
        node* it = root;

        while(it != nullptr) {
            if(it->get_key() > key)
                it = it->get_left();
            else if(it->get_key() < key)
                it = it->get_right();
            else
                break;
        }

        return it;
    }

    bool contains(int key) {return find(key) != nullptr;}

    bool is_equal(set& first, set& second) {
        std::queue<node*> f_queue;
        std::queue<node*> s_queue;

        if(first.root != nullptr)
            f_queue.push(first.root);
        if(second.root != nullptr)
            s_queue.push(second.root);

        while(f_queue.empty() == false && s_queue.empty() == false) {
            if(f_queue.front()->get_key() != s_queue.front()->get_key())
                return false;

            if(f_queue.front()->get_left() != nullptr)
                f_queue.push(f_queue.front()->get_left());
            if(f_queue.front()->get_right() != nullptr)
                f_queue.push(f_queue.front()->get_right());

            if(s_queue.front()->get_left() != nullptr)
                s_queue.push(s_queue.front()->get_left());
            if(s_queue.front()->get_right() != nullptr)
                s_queue.push(s_queue.front()->get_right());
        }

        return f_queue.empty() && s_queue.empty();
    }
    
};

int main(void) {

    return 0;
}