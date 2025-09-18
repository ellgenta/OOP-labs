#include <iostream>
#include <cassert>
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

    void set_left(node* l) {left = l;}

    void set_right(node* r) {right = r;}

    void set_parent(node* p) {parent = p;}

    void set_color(color c) {cl = c;}
};

class set {
private:
    node* root;
    size_t sz;

    void rotate_left(node* x) {
        node* z = x->get_left();
        node* g = x->get_parent()->get_parent();

        x->get_parent()->set_right(z);
        x->set_left(x->get_parent());

        if(g != nullptr) {
            if(g->get_left() == x->get_parent()) 
                g->set_left(x);
            else 
                g->set_right(x);
        } else {
            root = x;
        }

        x->get_parent()->set_parent(x);
        x->set_parent(g);
    }

    void rotate_right(node* x) {
        node* z = x->get_right();
        node* g = x->get_parent()->get_parent();

        x->get_parent()->set_left(z);
        x->set_right(x->get_parent());

        if(g != nullptr) {
            if(g->get_left() == x->get_parent()) 
                g->set_left(x);
            else 
                g->set_right(x);
        } else {
            root = x;
        }

        x->get_parent()->set_parent(x);
        x->set_parent(g);
    }   

    void fix_insert(node* it) {
        while(it->get_parent() && it->get_parent()->get_color() == red) {
            node* p = it->get_parent();
            if(p == p->get_parent()->get_left()) {
                node* uncle = p->get_parent()->get_right();
                if(uncle && uncle->get_color() == red) {
                    uncle->set_color(black);
                    p->set_color(black);
                    p->get_parent()->set_color(red);
                    it = p->get_parent();
                } else {
                    if(it == it->get_parent()->get_right()) {
                        it = it->get_parent();
                        rotate_left(it);
                    }
                    if(it->get_parent() && it->get_parent()->get_parent()) {
                        it->get_parent()->set_color(black);
                        it->get_parent()->get_parent()->set_color(red);
                        rotate_right(it->get_parent());
                    }
                }
            } else {
                node* uncle = it->get_parent()->get_parent()->get_left();
                if(uncle && uncle->get_color() == red) {
                    it->get_parent()->set_color(black);
                    uncle->set_color(black);
                    p->get_parent()->set_color(red);
                    it = it->get_parent()->get_parent();
                } else {
                    if(it == it->get_parent()->get_left()) {
                        it = it->get_parent();
                        rotate_right(it);
                    }
                    if(it->get_parent() && it->get_parent()->get_parent()) {
                        it->get_parent()->set_color(black);
                        it->get_parent()->get_parent()->set_color(red);
                        rotate_left(it->get_parent());
                    }
                }
            }
            if(it == root)
                break;
        }
        root->set_color(black);
    }

public:
    set() {root = nullptr; sz = 0;}

    set(const int* array, size_t size) {
        for(size_t i = 0; i < size; i++)
            insert(array[i]);
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
        if(root == nullptr){
            root = new node(key);
            sz += 1;
            return;   
        }

        node* it = root;
        while(it->get_key() != key) {
            if(it->get_key() > key) {
                if(it->get_left() == nullptr) {
                    it->set_left(new node(key, red, it));
                    break;
                }
                it = it->get_left();
            }
            else if(it->get_key() < key) {
                if(it->get_right() == nullptr) {
                    it->set_right(new node(key, red, it));
                    break;
                }
                it = it->get_right();
            }
        }

        if(it->get_key() == key)
            return;
        if(it->get_left() && it->get_left()->get_key() == key)
            it = it->get_left();
        else 
            it = it->get_right();

        fix_insert(it);
        sz += 1;
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
    set example;

    example.insert(1);
    example.insert(2);
    example.insert(3);
    example.insert(1);
    example.insert(0);
    example.insert(4);
    example.insert(5);
    example.insert(6);

    node* x = example.find(2);
    node* y = example.find(1);
    node* z = example.find(3);
    
    assert(example.find(4) == nullptr);
    assert(example.contains(3) == true);
    assert(example.size() == 3);

    return 0;
}