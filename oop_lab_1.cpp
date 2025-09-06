#include <cassert>

#define TESTS

struct node {
    int data;
    node* next;
};

class linked_list {
private:
    node* head;
    node* tail;
    size_t sz;
public:
    void initialize() {
        head = nullptr;
        tail = nullptr;
        sz = 0;
    }

    node* get_at(size_t pos) {
        if(pos >= sz)
            return nullptr;

        node* it = head;
        for(int i = 0; i < pos; i++)
            it = it->next;

        return it;
    }

    void push_front(int value) {
        node* new_head = new node;

        new_head->data = value;
        new_head->next = head;
        head = new_head;

        if(tail == nullptr)
            tail = head;

        sz += 1;
    }

    void pop_front() {
        if(sz == 0) 
            return;

        node* new_head = head->next;
        delete head;

        head = new_head;
        if(head == nullptr)
            tail = nullptr;
            
        sz -= 1;
    }

    void push_back(int value){
        node* new_tail = new node;

        new_tail->data = value;
        new_tail->next = nullptr;

        if(sz == 0) 
            head = new_tail;
        else 
            tail->next = new_tail;

        tail = new_tail;
        sz += 1;
    }

    void pop_back() {
        if(sz == 0)
            return;

        delete tail;

        if(sz == 1) {
            head = nullptr;
            tail = nullptr;
        } else {
            node* pred = this->get_at(sz-2);
            pred->next = nullptr;
            tail = pred;
        }

        sz -= 1;
    }

    void insert(node* prev, int value) {
        assert(prev);

        if(prev->next == nullptr) 
            this->push_back(value);
        else {
            node* new_node = new node;
            new_node->data = value;
            new_node->next = prev->next;
            prev->next = new_node;
            sz += 1;
        }
    }

    void erase(node* prev) {
        assert(prev);

        if(prev->next == nullptr) 
            return;

        node* succ = prev->next->next;
        if(succ == nullptr)
            this->pop_back();
        else {
            delete prev->next;
            prev->next = succ;
            sz -= 1;
        }
    }

    void resize(size_t new_size) {
        if(new_size == sz) 
            return;

        int diff = new_size - sz;
        if(diff > 0) {
            while(sz < new_size)
                push_back(0);
        } else {
            while(sz > new_size)
                pop_back();
        }
    }

    void clear() {
        while(sz > 0) 
            pop_back();
    }

    int& front() { assert(sz != 0); return head->data; }

    int& back() { assert(sz != 0); return tail->data; }

    size_t size() { return sz; }
};

#ifdef  TESTS

void test_initialize() {
    linked_list example;
    
    example.initialize();
    assert(example.size() == 0);
}

void test_push_front() {
    linked_list example;
    example.initialize();
    
    example.push_front(1);
    assert(example.size() == 1);
    assert(example.front() == example.back() == 1);
    
    example.push_front(2);
    assert(example.size() == 2);
    assert(example.front() == 2);
    assert(example.back() == 1);

    example.push_front(3);
    assert(example.size() == 3);
    assert(example.front() == 3);
    assert(example.back() == 1);

    example.clear();
}

void test_pop_front() {
    linked_list example;
    example.initialize();
    
    example.push_front(1);
    example.push_front(2);
    
    example.pop_front();
    assert(example.size() == 1);
    assert(example.front() == example.back() == 1);

    example.pop_front();
    assert(example.size() == 0);
}

void test_push_back() {
    linked_list example;
    example.initialize();
    
    example.push_back(1);
    assert(example.size() == 1);
    assert(example.front() == example.back() == 1);

    example.push_back(2);
    assert(example.size() == 2);
    assert(example.front() == 1);
    assert(example.back() == 2);

    example.push_back(3);
    assert(example.size() == 3);
    assert(example.front() == 1);
    assert(example.back() == 3);

    example.clear();
}

void test_pop_back() {
    linked_list example;
    example.initialize();

    example.push_back(1);
    example.push_back(2);
    example.push_back(3);

    example.pop_back();
    assert(example.size() == 2);
    assert(example.back() == 2);
    assert(example.front() == 1);

    example.pop_back();
    assert(example.size() == 1);
    assert(example.front() == example.back() == 1);

    example.pop_back();
    assert(example.size() == 0);
}

void test_insert() {
    linked_list example;
    example.initialize();

    example.push_back(1);

    example.insert(example.get_at(0), 3);
    assert(example.size() == 2);
    assert(example.front() == 1);
    assert(example.back() == 3);

    example.insert(example.get_at(0), 2);
    assert(example.size() == 3);
    assert(example.front() == 1);
    assert(example.back() == 3);
    
    example.pop_back();
    assert(example.back() == 2);

    example.clear();
}

void test_erase() {
    linked_list example;
    example.initialize();

    example.push_back(1);
    example.push_back(2);
    example.push_back(3);

    example.erase(example.get_at(0));
    assert(example.size() == 2);
    assert(example.front() == 1);
    assert(example.back() == 3);

    example.erase(example.get_at(0));
    assert(example.size() == 1);
    assert(example.front() == example.back() == 1);

    example.clear();
}

void test_front() {
    linked_list example;
    example.initialize();

    example.push_front(1);
    assert(example.front() == 1);

    example.push_back(2);
    assert(example.front() == 1);

    example.push_front(0);
    assert(example.front() == 0);

    example.front() = -1;
    assert(example.front() == -1);

    example.clear();
}

void test_back() {
    linked_list example;
    example.initialize();

    example.push_back(1);
    assert(example.back() == 1);

    example.push_front(0);
    assert(example.back() == 1);

    example.push_back(2);
    assert(example.back() == 2);

    example.back() = 22;
    assert(example.back() == 22);

    example.clear();
}

void test_resize() {
    linked_list example;
    example.initialize();
    
    example.resize(-1);
    assert(example.size() == 0);
    
    example.resize(3);
    assert(example.size() == 3);
    assert(example.front() == 0);
    assert(example.back() == 0);

    example.back() = 3; 
    example.resize(2);
    assert(example.size() == 2);
    assert(example.front() == 0);
    assert(example.back() == 0);
    
    example.resize(0);
    assert(example.size() == 0);
}

void test_size() {
    linked_list example;
    example.initialize();

    assert(example.size() == 0);
    
    example.push_back(1);
    assert(example.size() == 1);

    example.push_front(0);
    assert(example.size() == 2);

    example.push_back(2);
    assert(example.size() == 3);

    example.clear();
}

void test_clear() {
    linked_list example;
    example.initialize();

    example.clear();
    assert(example.size() == 0);

    example.push_back(1);
    example.push_back(1);
    example.push_back(1);

    example.clear();
    assert(example.size() == 0);

    example.push_back(1);

    example.clear();
    assert(example.size() == 0);
}

void test_get_at() {
    linked_list example;
    example.initialize();

    node* ptr;
    
    ptr = example.get_at(0);
    assert(ptr == nullptr);

    example.push_back(1);
    example.push_back(2);
    example.push_back(3);
    example.push_back(4);
    example.push_back(5);

    ptr = example.get_at(0);
    assert(ptr != nullptr);
    assert(ptr->data == 1);

    ptr = example.get_at(1);
    assert(ptr != nullptr);
    assert(ptr->data == 2);

    ptr = example.get_at(2);
    assert(ptr != nullptr);
    assert(ptr->data == 3);

    ptr = example.get_at(3);
    assert(ptr != nullptr);
    assert(ptr->data == 4);

    ptr = example.get_at(4);
    assert(ptr != nullptr);
    assert(ptr->data == 5);

    example.clear();
}

int main(void)
{
    test_initialize();
    test_pop_front();
    test_pop_back();
    test_push_front();
    test_push_back();
    test_insert();
    test_erase();
    test_front();
    test_back();
    test_resize();
    test_size();
    test_clear();
    test_get_at();

    return 0;
}

#endif