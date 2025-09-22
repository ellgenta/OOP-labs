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
    
    void fix_erase(node* it) {

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
            if(q.front()->left != nullptr)
                q.push(q.front()->left);
            if(q.front()->right != nullptr)
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

    set& operator=(const set& other) {
        if(&other == this) 
            return *this;

        if(root != nullptr) 
            clear();

        std::queue<node*> q;
        if(other.root != nullptr)
            q.push(other.root);

        while(q.empty() == false) {
            insert(q.front()->key);
            if(q.front()->left != nullptr)
                q.push(q.front()->left);
            if(q.front()->right != nullptr)
                q.push(q.front()->right);
            q.pop();
        }

        return *this;
    }

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
        if(root == nullptr) 
            return;

        node* it = root;
        while(it->key != key) {
            if(it->key > key && it->left != nullptr)
                it = it->left;
            else if(it->key < key && it->right != nullptr)
                it = it->right;
            else 
                return;
        }

        fix_erase(it);
        sz -= 1;
    }

    void swap(set& other) {
        std::swap(other.root, this->root);
        std::swap(other.sz, this->sz); 
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

    int count(int key) {return (int)contains(key);}

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
    set A;
    
    assert(A.find(0) == nullptr);
    assert(A.empty() == true);
    assert(A.size() == 0);

    const int* b = new const int[5] {1, 2, 3, 4, 5};
    set B(b, 5);

    assert(B.find(1) != nullptr);
    assert(B.find(5) != nullptr);
    assert(B.empty() == false);
    assert(B.size() == 5);

    set C(std::move(B));

    assert(B.empty() == true);
    assert(B.size() == 0);
    assert(C.empty() == false);
    assert(C.size() == 5);

    set D(C);
    assert(D.empty() == false);
    assert(D.size() == 5);
    assert(C.empty() == false);
    assert(C.size() == 5);
    assert(C.find(3) != nullptr);
    assert(D.find(3) != nullptr);
    assert(D.find(3) != C.find(3));

    set E;
    E = C;
    assert(E.find(1) != nullptr);
    assert(E.find(5) != nullptr);
    assert(E.empty() == false);
    assert(E.size() == 5);

    delete[] b;

    return 0;
}