#include <iostream>
#include <cassert>

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

class iterator {
private:
    node* ptr = nullptr;
    node* prev = nullptr;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = node;
    using pointer = node*;
    using reference = node&;
    
    iterator() {}
    
    iterator(const iterator& other) {
        ptr = other.ptr;  
        prev = other.prev;  
    }
    
    iterator(node* ptr_other, node* prev_other) {
        ptr = ptr_other;
        prev = prev_other;
    }
    
    ~iterator() = default;
    
    iterator& operator++() {
        iterator temp(*this);

        if(ptr == nullptr && prev == nullptr)
            return *this;
        if(ptr == nullptr && prev != nullptr) 
            ptr = prev->right;
        else if(ptr->parent == prev) {
            if(ptr->left != nullptr) {
                prev = ptr;
                ptr = ptr->left;
            }
            else if(ptr->left == nullptr && ptr->right == nullptr) {
                prev = ptr;
                ptr = ptr->parent;
            }
            else if(ptr->right == nullptr) {
                prev = ptr;
                ptr = ptr->right;
            }
        }
        else if(ptr->left == prev) {
            prev = ptr;
            ptr = ptr->right;
        }
        else if(ptr->right == prev) {
            prev = ptr;
            ptr = ptr->parent;
        }
        return temp;
    }

    iterator operator++(int) {
        if(ptr == nullptr && prev == nullptr)
            return *this;
        if(ptr == nullptr && prev != nullptr) 
            ptr = prev->right;
        else if(ptr->parent == prev) {
            if(ptr->left != nullptr) {
                prev = ptr;
                ptr = ptr->left;
            }
            else if(ptr->left == nullptr && ptr->right == nullptr) {
                prev = ptr;
                ptr = ptr->parent;
            }
            else if(ptr->right == nullptr) {
                prev = ptr;
                ptr = ptr->right;
            }
        }
        else if(ptr->left == prev) {
            prev = ptr;
            ptr = ptr->right;
        }
        else if(ptr->right == prev) {
            prev = ptr;
            ptr = ptr->parent;
        }
        return *this;
    }

    iterator &operator--();
    iterator operator--(int);
    int& operator*();
    int* operator->();
    friend bool operator==(const iterator &, const iterator &);
    friend bool operator!=(const iterator &, const iterator &);
};

class RB_Tree {
private:
    void rotate_left(node* x) {
        node* y = x->right;
        x->right = y->left;

        if(y->left != nullptr)
            y->left->parent= x;

        y->parent = x->parent;
        if(x->parent == nullptr)
            root = y;
        else if(x == x->parent->left)
            x->parent->left = y;
        else 
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }
    
    void rotate_right(node* x) {
        node* y = x->left;
        x->left = y->right;

        if(y->right != nullptr)
            y->right->parent= x;

        y->parent = x->parent;
        if(x->parent == nullptr)
            root = y;
        else if(x == x->parent->left)
            x->parent->left = y;
        else 
            x->parent->right = y;
            
        y->right = x;
        x->parent = y;
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
                        rotate_right(it->parent->parent);
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
                        rotate_left(it->parent->parent);
                    }
                }
            }
        }
        root->cl = black;
    }
    
    void transplant(node* u, node* v) {
        if(u->parent == nullptr)
            root = v;
        else if(u->parent->left == u)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    void fix_erase(node* it) {
        while(it != root && it->cl == black) {
            
        }
        it->cl = black;
    }

    node* get_min(node* x) {
        while(x->left != nullptr)
            x = x->left;
        return x;
    }

    void copy(node* src) {
        if(src == nullptr)
            return;
        insert(src->key);
        copy(src->left);
        copy(src->right);
    }

    void _clear(node* r) {
        if(r == nullptr)
            return;
        clear(r->left);
        clear(r->right);
        delete r;
    }
public:
    node* root = nullptr;

    RB_Tree() {}

    RB_Tree(RB_Tree& other) {copy(other.root);}

    RB_Tree(RB_Tree&& other) {
        root = other.root;
        other.root = nullptr;
    }

    ~RB_Tree() {clear(root);}

    RB_Tree& operator=(RB_Tree& other) {
        if(root != nullptr)
            clear(root);
        copy(other.root);
        return *this;
    }

    void clear(node* r) {
        _clear(r);
        r = nullptr;
    }

    node* find(node* r, int key) {
        if(r == nullptr)
            return nullptr;
        if(r->key < key)
            return find(r->right, key);
        else if(r->key > key)
            return find(r->left, key);
        return r;
    }

    bool insert(int key) {
        if(root == nullptr){
            root = new node(key);
            return true;   
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
            return false;
        if(it->left && it->left->key == key)
            it = it->left;
        else 
            it = it->right;

        fix_insert(it);
        return true;
    }

    bool erase(int key) {
        node* it = find(root, key);
        if(it == nullptr)
            return false;

        node* y = it;
        node* x = nullptr;
        color y_cl = y->cl;

        if(it->left == nullptr) {
            x = it->right;
            transplant(it, it->right);
        } else if(it->right == nullptr) {
            x = it->left;
            transplant(it, it->left);
        } else {
            y = get_min(it->right);
            y_cl = y->cl;
            x = y->right;

            if(y->parent == it) 
                x->parent = y;
            else {
                transplant(y, x);
                y->right = it->right;
                y->right->parent = y;
            }

            transplant(it, y);
            y->left = it->left;
            y->left->parent = y;
            y->cl = it->cl;
        }

        delete it;
        if(y_cl == black)
            fix_erase(x);
        return true;
    }

    bool is_equal(node* lhs, node* rhs) {
        if(!lhs && !rhs)
            return true;
        if(!lhs && rhs || lhs && !rhs)
            return false;
        if(lhs->key != rhs->key)
            return false;
        return is_equal(lhs->left, rhs->left) && is_equal(lhs->right, rhs->right);
    }  

    void swap(RB_Tree& other) {
        std::swap(other.root, this->root);
    }

    friend void swap(RB_Tree& lhs, RB_Tree& rhs) {
        RB_Tree temp = lhs;
        lhs = rhs;
        rhs = temp;
    }
};

class set {
private:
    RB_Tree tree;
    size_t sz = 0;

    void push_keys(node* r, int* cnt, size_t& pos) const {
        if(r == nullptr) return;
        push_keys(r->left, cnt, pos);
        cnt[pos++] = r->key;
        push_keys(r->right, cnt, pos);
    }
public:
    set() {}

    set(const int* array, size_t size) {
        for(size_t i = 0; i < size; i++)
            insert(array[i]);
    }

    set(set& other) {
        RB_Tree T(other.tree);
        tree = T;
        sz = other.sz;
    }

    set(set&& other) {
        this->tree.root = other.tree.root;
        this->sz = other.sz;
        other.tree.root = nullptr;
        other.sz = 0;
    }

    ~set() {}

    set& operator=(set& other) {
        if(&other != this) {
            set cp(other);
            swap(cp);
        }
        return *this;
    }

    set& operator=(set&& other) {
        swap(other);
        other.clear();
        return *this;
    }

    bool operator==(const set& other) {
        return is_equal(*this, other);
    }

    bool operator!=(const set& other) {
        return is_equal(*this, other) == false;
    }

    int operator<=>(const set& other) {
        int* this_keys = new int[this->sz];
        int* other_keys = new int[other.sz];
        
        size_t t_sz = 0;
        size_t o_sz = 0;
        push_keys(this->tree.root, this_keys, t_sz);
        push_keys(other.tree.root, other_keys, o_sz);
        
        for(size_t i = 0; i < t_sz && i < o_sz; i++) {
            if(this_keys[i] < other_keys[i]) return -1;
            if(this_keys[i] > other_keys[i]) return 1;
        }
        
        if(t_sz < o_sz) return -1;
        if(t_sz > o_sz) return 1;
        return 0;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const set& obj) {
        int* buf = new int[obj.sz];
        size_t p = 0;

        obj.push_keys(obj.tree.root, buf, p); 
        os << "[" << obj.sz << "]" << " { ";

        for(size_t i = 0; i < obj.sz; i++) {
            os << buf[i];
            if(i != obj.sz - 1)
                os << ", ";
            else 
                os << " ";
        }

        os << "}" << std::endl;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, set& obj) {
        size_t s = 0;
        is >> s;
        if(s != 0) {
            int key = 0;
            for(size_t i = 0; i < s; i++) {
                is >> key;
                obj.insert(key);
            }
        }
        return is;
    }
    
    node* find(int key) {return tree.find(tree.root, key);}

    void insert(int key) {sz += (int)tree.insert(key);}

    void erase(int key) {sz -= (int)tree.erase(key);}

    void clear() {
        tree.clear(tree.root);
        sz = 0;
    }

    bool empty() {return sz == 0;}

    size_t size() {return sz;}

    void swap(set& other) {
        //std::swap(this->tree, other.tree);
        std::swap(other.tree.root, this->tree.root);
        std::swap(other.sz, this->sz); 
    }

    friend void swap(set& lhs, set& rhs) {
        lhs.swap(rhs);
    }

    bool contains(int key) {return find(key) != nullptr;}

    int count(int key) {return (int)contains(key);}

    bool is_equal(const set& lhs, const set& rhs) {
        return tree.is_equal(lhs.tree.root, rhs.tree.root);
    }   
};

int main(void) {
    const int* b = new const int[5] {1, 3, 2, 4, 5};
    
    set A;
    set B(b, 5);
    set C = B;
    
    assert((A <=> B) == -1);
    assert((B <=> B) == 0);
    assert((B <=> C) == 0);
    assert((C <=> A) == 1);
    assert(B == C);
    assert(A != B);

    set D = std::move(C);
    assert(D == B);
    assert(A == C);

    set E;
    std::cin >> E;
    std::cout << A << B << E;

    delete[] b;

    return 0;
}