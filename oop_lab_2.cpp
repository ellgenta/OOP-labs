#include <iostream>
#include <cassert>
#include <queue>

class set {
private:
    using color = enum {red, black};

    struct node {
        int key = 0;
        color cl = black;
        node* left = nullptr;
        node* right = nullptr;
        node* parent = nullptr;

        node() {}

        node(int v) {key = v;}

        node(int v, color cl) {key = v; this->cl = cl;}

        node(int v, color cl, node* p) {key = v; this->cl = cl; parent = p;}
    };

    node* root = nullptr;
    size_t sz = 0;

    void rotate_left(node* x) {
        node* z = x->left;
        node* g = x->parent->parent;

        x->parent->right = z;
        x->left = x->parent;
        
        if(g != nullptr) {
            if(g->left == x->parent) 
                g->left = x;
            else 
                g->right = x;
        } else {
            root = x;
        }
        
        x->parent->parent = x;
        x->parent = g;
    }
    
    void rotate_right(node* x) {
        node* z = x->right;
        node* g = x->parent->parent;

        x->parent->left = z;
        x->right = x->parent;
        
        if(g != nullptr) {
            if(g->left == x->parent) 
            g->left = x;
            else 
            g->right = x;
        } else {
            root = x;
        }
        
        x->parent->parent = x;
        x->parent = g;
    }   
    
    void fix_insert(node* it) {
        while(it->parent && it->parent->cl == red) {
            node* p = it->parent;
            if(p == p->parent->left) {
                node* uncle = p->parent->right;
                if(uncle && uncle->cl == red) {
                    uncle->cl = black;
                    p->cl = black;
                    p->parent->cl = red;
                    it = p->parent;
                } else {
                    if(it == it->parent->right) {
                        it = it->parent;
                        rotate_left(it);
                    }
                    if(it->parent && it->parent->parent) {
                        it->parent->cl = black;
                        it->parent->parent->cl = red;
                        rotate_right(it->parent);
                    }
                }
            } else {
                node* uncle = it->parent->parent->left;
                if(uncle && uncle->cl == red) {
                    it->parent->cl = black;
                    uncle->cl = black;
                    p->parent->cl = red;
                    it = it->parent->parent;
                } else {
                    if(it == it->parent->left) {
                        it = it->parent;
                        rotate_right(it);
                    }
                    if(it->parent && it->parent->parent) {
                        it->parent->cl = black;
                        it->parent->parent->cl = red;
                        rotate_left(it->parent);
                    }
                }
            }
            if(it == root)
            break;
        }
        root->cl = black;
    }
    
public:
    set() {}

    set(const int* array, size_t size) {
        for(size_t i = 0; i < size; i++)
            insert(array[i]);
    }

    set(set& other) {
        std::queue<node*> q;
        if(other.root != nullptr)
            q.push(other.root);

        while(q.empty() == false) {
            insert(q.front()->key);
            if(q.front()->left == nullptr)
                q.push(q.front()->left);
            if(q.back()->right == nullptr)
                q.push(q.front()->right);
            q.pop();
        }
    }

    set(set&& other) {
        root = other.root;
        sz = other.sz;

        other.root = nullptr;
        other.sz = 0;
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
            if(q.front()->left != nullptr) 
                q.push(q.front()->left);
            if(q.front()->right != nullptr) 
                q.push(q.front()->right);
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
        while(it->key != key) {
            if(it->key > key) {
                if(it->left == nullptr) {
                    it->left = new node(key, red, it);
                    break;
                }
                it = it->left;
            }
            else if(it->key < key) {
                if(it->right == nullptr) {
                    it->right = new node(key, red, it);
                    break;
                }
                it = it->right;
            }
        }

        if(it->key == key)
            return;
        if(it->left && it->left->key == key)
            it = it->left;
        else 
            it = it->right;

        fix_insert(it);
        sz += 1;
    }

    void erase(int key) {

    }

    node* find(int key) {
        node* it = root;

        while(it != nullptr) {
            if(it->key > key)
                it = it->left;
            else if(it->key < key)
                it = it->right;
            else
                break;
        }

        return it;
    }

    bool contains(int key) {return find(key) != nullptr;}

    bool is_equal(set& first, set& second) {
        std::queue<std::pair<node*, node*>> q;

        if(first.root != nullptr && second.root != nullptr)
            q.push(std::make_pair(first.root, second.root));
        else if(first.root == nullptr && second.root == nullptr)
            return true;

        while(q.empty() == false) {
            node* f = q.front().first;
            node* s = q.front().second;

            if(f->key != s->key)
                return false;
            if(f->left && s->left)
                q.push(std::make_pair(f->left, s->left));
            else if(!f->left && s->left || !f->left && s->left)
                return false;
            if(f->right && s->right)
                q.push(std::make_pair(f->right, s->right));
            else if(!f->right && s->right || !f->right && s->right)
                return false;
        }

        return true;
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
    
    assert(example.find(4) != nullptr);
    assert(example.contains(3) == true);
    assert(example.size() == 7);

    return 0;
}