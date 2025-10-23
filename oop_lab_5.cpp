#include <iostream>
#include <compare>
#include <cassert>
#include <vector>

class RB_Tree {
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
        if(v != nullptr)
            v->parent = u->parent;
    }

    void fix_erase(node* it) {
        while(it != root && it->cl == black) {
            if(it->parent && it == it->parent->left) {
                node* w = it->parent->right;
                if(w && w->cl == red) {
                    w->cl = black;
                    it->parent->cl = red;
                    rotate_left(it->parent);
                    w = it->parent->right;
                }
                if(!w->left || w->left->cl == black && !w->right || w->right->cl == black) {
                    w->cl = red;
                    it = it->parent;
                }
                else {
                    if(!w->right || w->right->cl == black) {
                        w->left->cl = black;
                        w->cl = red;
                        rotate_right(w);
                        w = it->parent->right;
                    }
                    w->cl = it->parent->cl;
                    it->parent->cl = black;
                    w->right->cl = black;
                    rotate_left(it->parent);
                    it = root;
                }
            }
            else {
                node* w = it->parent->left;
                if(w && w->cl == red) {
                    w->cl = black;
                    it->parent->cl = red;
                    rotate_right(it->parent);
                    w = it->parent->left;
                }
                if(!w->right || w->right->cl == black && !w->left || w->left->cl == black) {
                    w->cl = red;
                    it = it->parent;
                }
                else {
                    if(!w->left || w->left->cl == black) {
                        w->right->cl = black;
                        w->cl = red;
                        rotate_left(w);
                        w = it->parent->left;
                    }
                    w->cl = it->parent->cl;
                    it->parent->cl = black;
                    w->left->cl = black;
                    rotate_right(it->parent);
                    it = root;
                }
            }
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
        using value_type = int;
        
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

            if(curr->right == nullptr) {
                prev = curr;
                while(curr && curr->key <= prev->key)
                    curr = curr->parent;
            } 
            else {
                prev = curr;
                curr = curr->right;
                while(curr->left != nullptr)
                    curr = curr->left;
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

            if(curr->left == nullptr) {
                prev = curr;
                while(curr && curr->key >= prev->key)
                    curr = curr->parent;
            } 
            else {
                prev = curr;
                curr = curr->left;
                while(curr->right != nullptr)
                    curr = curr->right;
            }
            
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        const int& operator*() {
            assert(curr != nullptr);
            return curr->key;
        }

        const node* operator->() {
            return curr;
        }

        iterator& operator=(iterator &other) {
            curr = other.curr;
            prev = other.prev; 
            return other;
        }

        friend bool operator==(const iterator &lhs, const iterator &rhs) {
            return lhs.curr == rhs.curr && lhs.prev == rhs.prev;
        }

        friend bool operator!=(const iterator &lhs, const iterator &rhs) {
            return lhs.curr != rhs.curr && lhs.prev != rhs.prev;
        }
    };

    class reverse_iterator {
    private:
        iterator it;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = int;
        
        reverse_iterator() {}

        reverse_iterator(const iterator& other) : it(other) {}
        
        reverse_iterator(const reverse_iterator& other) : it(other.it) {}

        reverse_iterator(const reverse_iterator&& other) : it(other.it) {}
        
        reverse_iterator(node* ptr_other, node* prev_other) : it(ptr_other, prev_other) {}
        
        ~reverse_iterator() = default;
        
        reverse_iterator& operator++() {
            --it;
            return *this;
        }

        reverse_iterator operator++(int) {
            it--;
            return *this;
        }

        reverse_iterator& operator--() {
            ++it;
            return *this;
        }

        reverse_iterator operator--(int) {
            it++;
            return *this;
        }

        reverse_iterator& operator=(reverse_iterator& other) {
            it = other.it;
            return *this;
        }

        const int& operator*() {
            return *it;
        }

        /*
        const node* operator->() {
            return it->;
        }
        */

        friend bool operator==(const reverse_iterator &lhs, const reverse_iterator &rhs) {
            return lhs.it == rhs.it;
        }

        friend bool operator!=(const reverse_iterator &lhs, const reverse_iterator &rhs) {
            return lhs.it != rhs.it;
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

    iterator find(int key) const {
        iterator it = begin();
        iterator en = end();
        while(it != en && *it != key)
            it++;
        return it;
    }

    node* find(node* r, int key) const {
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

    bool is_equal(node* lhs, node* rhs) const {
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

    iterator begin() const {
        node* temp = root;
        while(temp != nullptr && temp->left != nullptr)
            temp = temp->left;
        iterator t{temp, nullptr};
        return t;
    }

    iterator end() const {  
        node* temp = root;
        while(temp != nullptr && temp->right != nullptr)
            temp = temp->right;
        iterator t{nullptr, temp};
        return t;
    }

    reverse_iterator rbegin() const {
        node* temp = root;
        while(temp != nullptr && temp->right != nullptr)
            temp = temp->right;
        iterator t{temp, temp != nullptr ? temp->parent : nullptr};
        return reverse_iterator(t);
    }

    reverse_iterator rend() const {  
        node* temp = root;
        while(temp != nullptr && temp->left != nullptr)
            temp = temp->left;
        iterator t{nullptr, temp};
        return reverse_iterator(t);
    }
};

class abstract_data_t {
public:
    virtual ~abstract_data_t() = 0;
    virtual void insert(int) = 0;
    virtual void insert_range(std::vector<int>&&) = 0;
    virtual void erase(int) = 0;
    virtual bool contains(int) const = 0;
    virtual int count(int) const = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
    virtual RB_Tree::iterator find(int) const = 0;
    virtual RB_Tree::iterator begin() const = 0;
    virtual RB_Tree::iterator end() const = 0;
    virtual RB_Tree::reverse_iterator rbegin() const = 0;
    virtual RB_Tree::reverse_iterator rend() const = 0;
    virtual bool operator==(const abstract_data_t&) const = 0;
    abstract_data_t& operator=(abstract_data_t&); 
};

inline abstract_data_t::~abstract_data_t() {}

class set : public abstract_data_t {
private:
    RB_Tree tree;
    size_t sz = 0;
public:
    using iterator = RB_Tree::iterator;
    using reverse_iterator = RB_Tree::reverse_iterator;

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
        while(st != fn) {
            insert(*st);
            st++;
        }
    }

    set(reverse_iterator st, reverse_iterator fn) {
        while(st != fn) {
            insert(*st);
            st++;
        }
    }

    ~set() {}

    /*
    set& operator=(set& other) {
        if(&other != this) {
            set cp(other);
            swap(cp);
        }
        return *this;
    }
    */

    abstract_data_t& operator=(const abstract_data_t& other) override {
        auto _other = dynamic_cast<const set*>(&other);
        if(_other != this) {
            set temp(*_other);
            this->swap(temp);
            /*
            this->tree = _other->tree;
            this->sz = _other->sz;
            */
        }
        return *this;
    }
    
    set& operator=(set&& other) {
        swap(other);
        other.clear();
        return *this;
    }

    /*
    bool operator==(const set& other) const {
        return is_equal(*this, other);
    }
    */

    bool operator==(const abstract_data_t& other) const override {
        const auto _other = dynamic_cast<const set*>(&other);
        if (_other == nullptr) {
            return false;  
        }
        return is_equal(*this, *_other);
    }   
    
    virtual bool operator!=(const set& other) const {
        return !(*this == other);
    }

    friend std::strong_ordering operator<=>(const set& t, const set& other) {
        auto t_end(t.end()), other_end(other.end());
        auto t_it(t.begin()), other_it(other.begin());

        while(t_it != t_end && other_it != other_end) {
            if(*t_it < *other_it)
                return std::strong_ordering::less;
            if(*t_it > *other_it)
                return std::strong_ordering::greater;
            t_it++;
            other_it++;
        }

        if(t_it == t.end() && other_it != other.end())
            return std::strong_ordering::less;
        if(t_it != t.end() && other_it == other.end())
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const set& obj) {
        os << "[" << obj.sz << "]" << " { ";
        auto en = obj.end();
        auto it = obj.begin();

        while(it != en) {
            os << *it;
            it++;
            if(it != en)
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
    
    RB_Tree::iterator find(int key) const override {return tree.find(key);}

    void insert(int key) override {sz += (int)tree.insert(key);}

    void insert_range(std::vector<int>&& rg) override {
        for(auto elem : rg)
            sz += (int)tree.insert(elem);
    }

    void erase(int key) override {sz -= (int)tree.erase(key);}

    void clear() {
        tree.clear();
        sz = 0;
    }

    bool empty() const override {return sz == 0;}

    size_t size() const override {return sz;}

    void swap(set& other) {
        std::swap(other.tree.root, this->tree.root);
        std::swap(other.sz, this->sz); 
    }

    friend void swap(set& lhs, set& rhs) {
        lhs.swap(rhs);
    }

    bool contains(int key) const override {return find(key) != end();}

    int count(int key) const override {return (int)contains(key);}

    bool is_equal(const set& lhs, const set& rhs) const {
        return tree.is_equal(lhs.tree.root, rhs.tree.root);
    }   

    RB_Tree::iterator begin() const override {return tree.begin();}

    RB_Tree::iterator end() const override {return tree.end();}

    RB_Tree::reverse_iterator rbegin() const override {return tree.rbegin();}

    RB_Tree::reverse_iterator rend() const override {return tree.rend();}
};

using container = set;

int main() {
    abstract_data_t *v = new container({2, 3, 5, 7});
    assert(!v->empty());
    v->insert(11);
    assert(v->contains(11) == true);
    v->insert_range({13, 17, 19});
    assert(8 == v->size());
    abstract_data_t *w = new container();
    assert(!w->size());
    *w = *v;
    //assert(*w == *v);
    /*
    w->erase(19);
    assert(0 == *w->begin());
    assert(17 == *w->rbegin());
    assert(7 == w->size());
    */
    delete v;
    delete w;
}