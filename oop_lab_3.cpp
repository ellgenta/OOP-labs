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

    struct custom_queue {
        node* head = nullptr;
        node* tail = nullptr;

        custom_queue() {}

        custom_queue(node* head) {
            this->head = head;
            this->tail = head;
        }

        void push(int& key) {
            if(head == nullptr) {
                head = new node(key);
                tail = new node(key);
                return;
            }
            node* t = new node(key);
            t->left = tail;
            tail->right = t->left;
            tail = t;
        }

        void pop() {
            if(tail == head)
                tail = nullptr;
            node* x = head->right;
            delete head;
            head = x;
        }
    
        int front() {
            assert(head != nullptr);
            return head->key;
        }

        int back() {
            assert(tail != nullptr);
            return head->key;
        }

        bool empty() {return head == nullptr;}
    }; 

    node* root = nullptr;
    size_t sz = 0;

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

    void traverse(node* x, std::queue<int>& o) {
        if(x == nullptr)
            return;
        traverse(x->left, o);
        o.push(x->key);
        traverse(x->right, o);
    }
public:
    set() {}

    set(const int* array, size_t size) {
        for(size_t i = 0; i < size; i++)
            insert(array[i]);
    }

    set(std::vector<int>::iterator first, std::vector<int>::iterator last) {
        while(first != last) {
            insert(*first);
            first += 1;
        }
    }

    set(set& other) {
        std::queue<node*> q; //update
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

    bool operator==(const set& other) {
        return is_equal(*this, other);
    }

    bool empty() {return sz == 0;}

    size_t size() {return sz;}

    void clear() {
        if(root == nullptr)
            return;

        std::queue<node*> q; //update, clear
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
        node* it = find(key);
        if(it == nullptr)
            return;

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

    bool is_equal(const set& first, const set& second) {
        if(first.root == second.root)
            return true;

        std::queue<int> f;
        std::queue<int> s;

        traverse(first.root, f);
        traverse(second.root, s);

        return f == s;
    }   
};

int main(void) {
    set A;
    
    assert(A.find(0) == nullptr);
    assert(A.empty() == true);
    assert(A.size() == 0);

    const int* b = new const int[5] {1, 3, 2, 4, 5};
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

    std::vector<int> v = {5, 3, 2, 1, 4, 7, 6};
    set F(v.begin(), v.begin()+5);

    assert(F.empty() == false);
    assert(F.size() == 5);
    assert(F == E);

    delete[] b;

    return 0;
}