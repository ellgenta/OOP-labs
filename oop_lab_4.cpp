#include <iostream>
#include <compare>
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

class RB_Tree {
private:
    node* max = nullptr;

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
        _clear(r->left);
        _clear(r->right);
        delete r;
    }
public:
    class iterator {
    private:
        node* curr = nullptr;
        node* prev = nullptr;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = node;
        
        iterator() {}
        
        iterator(const iterator& other) {
            curr = other.curr;  
            prev = other.prev;  
        }

        iterator(const iterator&& other) {
            curr = other.curr;  
            prev = other.prev;  
        }
        
        iterator(node* ptr_other, node* prev_other) {
            curr = ptr_other;
            prev = prev_other;
        }
        
        ~iterator() = default;
        
        iterator& operator++() {
            if(curr == nullptr)
                return *this;
            else if(curr->left == nullptr && curr->right == nullptr) {
                prev = curr;
                while(curr && curr->key <= prev->key)
                    curr = curr->parent;
            } 
            else if(curr->right != nullptr) {
                prev = curr;
                curr = curr->right;
                while(curr->left != nullptr)
                    curr = curr->left;
            }
            else if(prev == curr->left && curr->right == nullptr) {
                prev = curr;
                while(curr && curr->key <= prev->key)
                    curr = curr->parent;
            } 
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        iterator &operator--() {
            if(curr == nullptr) {
                curr = prev;
                return *this;
            }

            else if(curr->left == nullptr && curr->right == nullptr) {
                prev = curr;
                while(curr && curr->key >= prev->key)
                    curr = curr->parent;
            } 
            else if(curr->left != nullptr) {
                prev = curr;
                curr = curr->left;
                while(curr->right != nullptr)
                    curr = curr->right;
            }
            else if(prev == curr->right && curr->left == nullptr) {
                prev = curr;
                while(curr && curr->key >= prev->key)
                    curr = curr->parent;
            } 
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        iterator& operator=(iterator& other) {
            curr = other.curr;
            prev = other.prev;
            return *this;
        }

        const int& operator*() {
            assert(curr != nullptr);
            return curr->key;
        }

        const node* operator->() {
            return curr;
        }

        friend bool operator==(const iterator &lhs, const iterator &rhs) {
            return lhs.curr == rhs.curr;
        }

        friend bool operator!=(const iterator &lhs, const iterator &rhs) {
            return lhs.curr != rhs.curr;
        }
    };
    node* root = nullptr;

    RB_Tree() {}

    RB_Tree(RB_Tree& other) {copy(other.root);}

    RB_Tree(RB_Tree&& other) {
        root = other.root;
        other.root = nullptr;
    }

    ~RB_Tree() {_clear(root);}

    RB_Tree& operator=(RB_Tree& other) {
        if(root != nullptr)
            {_clear(root); root=nullptr;}
        copy(other.root);
        return *this;
    }

    void clear() {
        _clear(root);
        root = nullptr;
    }

    iterator find(int key) {
        iterator it = begin();
        iterator en = end();
        while(it != en && *it != key)
            it++;
        return it;
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
        if((!lhs && rhs) || (lhs && !rhs))
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

    iterator begin() {
        node* temp = root;
        while(temp != nullptr && temp->left != nullptr)
            temp = temp->left;
        iterator t{temp, nullptr};
        return t;
    }

    iterator end() {  
        node* temp = root;
        while(temp != nullptr && temp->right != nullptr)
            temp = temp->right;
        iterator t{nullptr, temp};
        return t;
    }

    iterator rbegin() {
        node* temp = root;
        while(temp != nullptr && temp->left != nullptr)
            temp = temp->left;
        iterator t{nullptr, temp};
        return t;
    }

    iterator rend() {  
        node* temp = root;
        while(temp != nullptr && temp->right != nullptr)
            temp = temp->right;
        iterator t{temp, temp != nullptr ? temp->parent : nullptr};
        return t;
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
    using iterator = RB_Tree::iterator;

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

    set(std::initializer_list<int> l) {
        for(auto n : l)
            insert(n);
    }

    set(iterator st, iterator fn) {
        while(st != fn) 
            insert(*st++);
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
        return !(*this == other);
    }

    friend std::strong_ordering operator<=>(const set& t, const set& other) {
        int* this_keys = new int[t.sz];
        int* other_keys = new int[other.sz];
        
        size_t t_sz = 0;
        size_t o_sz = 0;
        t.push_keys(t.tree.root, this_keys, t_sz);
        t.push_keys(other.tree.root, other_keys, o_sz);
        
        for(size_t i = 0; i < t_sz && i < o_sz; i++) {
            if(this_keys[i] < other_keys[i]) 
                return std::strong_ordering::less;
            if(this_keys[i] > other_keys[i]) 
                return std::strong_ordering::greater;
        }
        
        if(t_sz < o_sz) return std::strong_ordering::less;
        if(t_sz > o_sz) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
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
    
    RB_Tree::iterator find(int key) {return tree.find(key);}

    void insert(int key) {sz += (int)tree.insert(key);}

    void erase(int key) {sz -= (int)tree.erase(key);}

    void clear() {
        tree.clear();
        sz = 0;
    }

    bool empty() {return sz == 0;}

    size_t size() {return sz;}

    void swap(set& other) {
        std::swap(other.tree.root, this->tree.root);
        std::swap(other.sz, this->sz); 
    }

    friend void swap(set& lhs, set& rhs) {
        lhs.swap(rhs);
    }

    bool contains(int key) {return find(key) != end();}

    int count(int key) {return (int)contains(key);}

    bool is_equal(const set& lhs, const set& rhs) {
        return tree.is_equal(lhs.tree.root, rhs.tree.root);
    }   

    RB_Tree::iterator begin() {return tree.begin();}

    RB_Tree::iterator end() {return tree.end();}

    RB_Tree::iterator rbegin() {return tree.rbegin();}

    RB_Tree::iterator rend() {return tree.rend();}
};

int main(void) {
    const int* b = new const int[5] {1, 3, 2, 4, 5};
    
    set A;
    set B(b, 5);
    set C = B;

    set::iterator _a = B.find(1);
    set::iterator _b = B.find(3);
    set::iterator _c = B.find(2);
    set::iterator _d = B.find(4);
    set::iterator _e = B.find(5);
    set::iterator _f = B.find(0);

    assert(_a == B.begin());
    assert(*_a == 1);
    assert(*_b == 3);
    assert(*_c == 2);
    assert(*_d == 4);
    assert(*_e == 5);
    assert(_f == B.end());

    for(auto n : B)
        std::cout << n << " ";

    std::cout << "\n";

    for(auto it = B.rend(); it != B.rbegin(); --it) 
        std::cout << *it << " ";

    delete[] b;

    std::cout << "\n";

    set __a({19, 47, 74, 91});
    for (auto it = __a.begin(); it != __a.end(); ++it) std::cout << *it << " ";

    std::cout << "\n";

    set __b(__a.begin(), __a.end());
    assert(__a == __b);
    for (auto &&it : __b) std::cout << it << " ";

    std::cout << "\n";

    //set c(__a.rbegin(), __a.rend());
    //for (auto &&it : c) std::cout << it << " ";

    return 0;
}