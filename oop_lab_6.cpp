#include <iostream>
#include <compare>
#include <cassert>
#include <vector>

template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class RB_Tree {
private:
    using color = enum {red, black};
    
    struct node {
        T key;
        color cl = black;
        node* left = nullptr;
        node* right = nullptr;
        node* parent = nullptr;
    
        node() {}
    
        node(T v) {key = v;}
    
        node(T v, color cl, node* p) {key = v; this->cl = cl; parent = p;}
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
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = std::bidirectional_iterator_tag;
    
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

        const T& operator*() {
            assert(curr != nullptr);
            return curr->key;
        }

        const T* operator->() {
            return &curr->key;
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
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;
        
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

        const T& operator*() {
            return *it;
        }

        /*
        const int* operator->() {
            return it*;
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

    iterator find(const T& key) const {
        iterator it = begin();
        iterator en = end();
        while(it != en && *it != key)
            it++;
        return it;
    }

    node* find(node* r, const T& key) const {
        if(r == nullptr)
        return nullptr;
        if(r->key < key)
        return find(r->right, key);
        else if(r->key > key)
        return find(r->left, key);
        return r;
    }

    bool insert(const T& key) {
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

    bool erase(const T& key, iterator& pos) {
        pos = find(key);
        node* it = pos.curr;
        //node* it = find(root, key);

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

template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class set {
private:
    RB_Tree<T, Compare, Allocator> tree;
    size_t sz = 0;
public:
    using iterator = RB_Tree<T, Compare, Allocator>::iterator;
    using reverse_iterator = RB_Tree<T, Compare, Allocator>::reverse_iterator;

    set() {}

    set(const int* array, size_t size) {
        for(size_t i = 0; i < size; i++)
            insert(array[i]);
    }
   
    set(set& other) {
        RB_Tree<T, Compare, Allocator> _T(other.tree);
        tree = _T;
        sz = other.sz;
    }

    set(set&& other) {
        this->tree.root = other.tree.root;
        this->sz = other.sz;
        other.tree.root = nullptr;
        other.sz = 0;
    }

    set(std::initializer_list<T> l) {
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

    set& operator=(set& other) {
        if(this != &other) {
            this->clear();
            for(auto el : other)
                insert(el);
        }
        return *this;
    }
    
    set& operator=(set&& other) {
        swap(other);
        other.clear();
        return *this;
    }

    bool operator==(const set& other) const {
        auto o_it = other.begin();
        auto it = this->begin();
        for(; it != this->end() && o_it != other.end(); it++, o_it++) {
            if(*it != *o_it) 
                return false;
        }
        return it == this->end() && o_it == other.end();
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
    
    iterator find(const T& key) const {return tree.find(key);}

    void insert(T key) {sz += (int)tree.insert(key);}

    void insert(iterator pos, const T& key) {insert(key);}

    void insert(reverse_iterator pos, const T& key) {insert(key);}

    /*
    void erase(T key) {
        //iterator it = this->end();
        sz -= (int)tree.erase(key);
    }
    */
    
    size_t erase(const T& key) {
        iterator it = this->end();
        sz -= (int)tree.erase(key, it);
        return sz;
    }

    void erase(iterator pos) {
        iterator& it = this->end();
        sz -= (int)tree.erase(*pos);
        erase(*pos);
    }

    void erase(reverse_iterator pos) {erase(*pos);}

    void clear() {
        tree.clear();
        sz = 0;
    }

    void merge(set<T, Compare, Allocator> other) {
        for(auto& elm : other) 
            this->insert(elm);
        other.clear();
    }

    bool empty() const {return sz == 0;}

    size_t size() const {return sz;}

    void swap(set& other) {
        std::swap(other.tree.root, this->tree.root);
        std::swap(other.sz, this->sz); 
    }

    friend void swap(set& lhs, set& rhs) {
        lhs.swap(rhs);
    }

    bool contains(T key) const {return find(key) != end();}

    int count(T key) const {return (int)contains(key);}

    bool is_equal(const set& lhs, const set& rhs) const {
        return tree.is_equal(lhs.tree.root, rhs.tree.root);
    }   

    iterator begin() const {return tree.begin();}

    iterator end() const {return tree.end();}

    reverse_iterator rbegin() const {return tree.rbegin();}

    reverse_iterator rend() const {return tree.rend();}
};

#define container   set

//using container = set;

int main() {
    //Assertions part I:
    
    container<int> a({2, 3, 5, 7});
    
    a.insert(11);
    assert(a.find(11) == a.rbegin());
    a.insert(a.begin(), 1);
    assert(1 == *a.begin());
    //a.extend(container<int>({13, 17, 19})); 
    a.merge(container<int>({13, 17, 19}));
    a.erase(a.rbegin());
    assert(a.find(19) == a.end());
    assert(8 == a.size());
    container<char> b = {'a','b','r','a','\0'}; //initially "abra"
    assert(0 == *b.rbegin());
    b.erase(b.rbegin());
    b.insert('c');
    assert('c' == *b.rbegin());
    b.insert(b.end(), 'a');
    assert('a' == *b.rend());
    b.merge(container<char>({'d','a','b','r','a'})); //"dabra"
    assert(b.rbegin() == b.find(0));
    //b[0] = 'A';
    //assert('A' == b.front());
    container<std::string> c({"Hello", "world"});
    //c[0].append(",");
    c.insert(c.find("world"), " ");
    //c[c.size() - 1].append("!");
    
    
    //Assertion part II:
    set<int> s1;
    
    s1.insert(3);
    s1.insert(3);
    assert(1 == s1.count(3));
    
    set<int> s2 = {1, 2, 3, 4, 1, 2, 3, 4};
    assert(4 == s2.size());
    
    for (auto it = s2.begin(); it != s2.end();)
    {
        if (*it % 2)
        //it = s2.erase(it);
        //else
        ++it;
         
    }
    assert(0 == s2.erase(1));
    /* control line
    */
    
    return 0;
}