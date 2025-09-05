#include <cstdio>
#include <cstdlib>
#include <cassert>

#define TESTS

using signal = enum {
    SIG_DEF,
    SIG_OS,
    SIG_TRN,
    SIG_IC
};

signal received = SIG_DEF;

void error_handling(signal name){
    #if !defined TESTS
    if(name==SIG_OS)
        perror("SIG_OS: The operating system did not allocate the memory\n");
    else if(name==SIG_TRN)
        perror("SIG_TRN: Passing argument is transcendent\n");
    else if(name==SIG_IC)
        perror("SIG_IC: Function can not be called\n");
    #endif
    received=name;
}

struct node {
    int data;
    node *next;
};

class linked_list {
private:
    node* head;
    node* tail;
    size_t sz;
public:
    void initialize() {
        head = NULL;
        tail = NULL;
        sz = 0;

        received = SIG_DEF;
    }

    void push_front(int value) {
        node* new_head = (node*)malloc(sizeof(node));
        if(new_head == nullptr){
            error_handling(SIG_OS);
            return;
        }

        new_head->data = value;
        new_head->next = head;
        head = new_head;
        if(tail == nullptr)
            tail = head;

        sz += 1;

        received = SIG_DEF;
    }

    void pop_front() {
        if(sz == 0){
            error_handling(SIG_IC);
            return;
        }

        node* new_head = head->next;
        free(head);
        head = new_head;
        if(head == nullptr)
            tail = nullptr;
        sz -= 1;

        received = SIG_DEF;
    }

    void push_back(int value){
        node *new_tail = (node*)calloc(1, sizeof(node));
        if(new_tail == nullptr){
            error_handling(SIG_OS);
            return;
        }

        new_tail->data=value;
        if(head)
            tail->next = new_tail;
        else
            head = new_tail;
        tail = new_tail;

        sz += 1;

        received = SIG_DEF;
    }

    void pop_back() {
        if(sz == 0) {
            error_handling(SIG_IC);
            return;
        }

        if(head == tail) {
            free(head);
            head = nullptr;
            tail = nullptr;
            sz -= 1;

            received = SIG_DEF;
            return;
        }

        node* tmp = head;
        while(tmp->next != tail)
            tmp = tmp->next;

        free(tail);
        tail = tmp;
        tail->next = NULL;
        sz -= 1;

        received=SIG_DEF;
    }

    void insert(node* prev, int value) {
        if(prev->next == nullptr){
            this->push_back(value);
            return;
        }

        node* new_node = (node*)malloc(sizeof(node));
        if(new_node == nullptr) {
            error_handling(SIG_OS);
            return;
        }

        new_node->data = value;
        new_node->next = prev->next;
        prev->next = new_node;

        sz += 1;

        received = SIG_DEF;
    }

    void erase(node* prev) {
        if(prev->next == nullptr){
            received = SIG_DEF;
            return;
        }

        node* aux_ptr = prev->next->next;
        if(aux_ptr == nullptr)
            tail = head;
        free(prev->next);
        prev->next=aux_ptr;
        sz -= 1;

        if(sz == 0){
            head = nullptr;
            tail = nullptr;
        }

        received = SIG_DEF;
    }

    int front() {
        if(head == nullptr) {
            error_handling(SIG_IC);
            #if !defined TESTS
            #error Returning value can not be used!
            #else
            return SIG_IC;
            #endif
        }
        received = SIG_DEF;

        return head->data;
    }

    int back(){
        if(tail == nullptr) {
            error_handling(SIG_IC);
            #if !defined TESTS
            #error Returning value can not be used!
            #else
            return SIG_IC;
            #endif
        }
        received = SIG_DEF;

        return tail->data;
    }

    void resize(int newsize){
        if(newsize < 0){
            error_handling(SIG_TRN);
            return;
        }

        if(newsize == (int)sz){
            received = SIG_DEF;
            return;
        }

        int diff = newsize - (int)sz;
        if(diff > 0){
            while(sz < newsize)
                push_back(0);
        }else{
            while(sz > newsize)
                pop_back();
        }

        received = SIG_DEF;
    }
    
    size_t size(){
        received = SIG_DEF;

        return sz;
    }

    void clear(){
        if(head == nullptr)
            return;

        node* aux_ptr = head;
        node* aux_ptr_next = aux_ptr;

        while(aux_ptr_next){
            aux_ptr_next = aux_ptr->next;
            free(aux_ptr);
            aux_ptr = aux_ptr_next;
            sz -= 1;
        }

        head = aux_ptr;
        tail = aux_ptr;

        received = SIG_DEF;
    }
};


#define DUMMY_MACRO
#ifndef DUMMY_MACRO

void test_initialize(){
    linked_list example;

    initialize(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_push_front(){
    linked_list example={NULL,NULL,0};

    push_front(&example,1);
    assert(received==SIG_DEF);
    assert(example.head);
    assert(example.tail);
    assert(example.head==example.tail);
    assert(example.head->data==1);
    assert(example.size==1);

    push_front(&example,2);
    assert(received==SIG_DEF);
    assert(example.head!=example.tail);
    assert(example.head->next==example.tail);
    assert(example.head->data==2);
    assert(example.tail->data==1);
    assert(example.size==2);

    free(example.tail);
    free(example.head);
}

void test_pop_front(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),2};
    assert(example.head);
    assert(example.tail);
    example.head->next=example.tail;
    example.head->data=1;

    pop_front(&example);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.head->next==NULL);
    assert(example.head->data==0);
    assert(example.size==1);

    pop_front(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    pop_front(&example);
    assert(received==SIG_IC);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_push_back(){
    linked_list example={NULL,NULL,0};

    push_back(&example,1);
    assert(received==SIG_DEF);
    assert(example.head);
    assert(example.head==example.tail);
    assert(example.head->data==1);
    assert(example.size==1);

    push_back(&example,2);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==1);
    assert(example.tail->data==2);
    assert(example.size==2);

    free(example.tail);
    free(example.head);
}

void test_pop_back(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),2};
    assert(example.head);
    assert(example.tail);
    example.head->data=1;
    example.head->next=example.tail;

    pop_back(&example);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.tail->data==1);
    assert(example.size==1);

    pop_back(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    pop_back(&example);
    assert(received==SIG_IC);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_insert(){
    linked_list example;
    example.head=calloc(1,sizeof(node));
    assert(example.head);
    example.tail=example.head;
    example.size=1;

    insert(example.head,2,&example);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==0);
    assert(example.tail->data==2);
    assert(example.size==2);

    insert(example.head,1,&example);
    assert(received==SIG_DEF);
    assert(example.head->next->next==example.tail);
    assert(example.head->next->data==1);
    assert(example.tail->data==2);

    free(example.tail);
    free(example.head->next);
    free(example.head);
}

void test_erase(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),4};
    node* sec=malloc(sizeof(node));
    node* thrd=malloc(sizeof(node));
    assert(example.head);
    assert(example.tail);
    assert(sec);
    assert(thrd);
    thrd->next=example.tail;
    thrd->data=1;
    sec->next=thrd;
    sec->data=2;
    example.head->next=sec;
    example.head->data=3;

    erase(example.tail,&example);
    assert(received==SIG_DEF);
    assert(example.head->next==sec);
    assert(sec->next==thrd);
    assert(example.tail==thrd->next);
    assert(example.size==4);

    erase(example.head->next,&example);
    assert(received==SIG_DEF);
    assert(example.head->next->next==example.tail);
    assert(example.head->next->data==2);
    assert(example.tail->data==0);
    assert(example.tail->next==NULL);
    assert(example.size==3);

    erase(example.head,&example);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==3);
    assert(example.tail->data==0);
    assert(example.tail->next==NULL);
    assert(example.size==2);

    erase(example.head,&example);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.head->data==3);
    assert(example.tail->next==NULL);
    assert(example.size==1);
}

void test_front(){
    linked_list example={NULL,NULL,0};

    assert(front(&example)==SIG_IC);
    assert(received==SIG_IC);

    example.head=malloc(sizeof(node));
    assert(example.head);
    example.head->next=NULL;
    example.tail=example.head;
    example.head->data=1;
    example.size=1;
    assert(front(&example)==1);
    assert(received==SIG_DEF);

    example.tail=malloc(sizeof(node));
    assert(example.tail);
    example.size=2;
    example.head->next=example.tail;
    example.tail->next=NULL;
    example.tail->data=2;
    assert(front(&example)==1);
    assert(received==SIG_DEF);
}

void test_back(){
    linked_list example={NULL,NULL,0};

    assert(back(&example)==SIG_IC);
    assert(received==SIG_IC);

    example.head=malloc(sizeof(node));
    assert(example.head);
    example.head->next=NULL;
    example.tail=example.head;
    example.head->data=1;
    example.size=1;
    assert(back(&example)==1);
    assert(received==SIG_DEF);

    example.tail=malloc(sizeof(node));
    assert(example.tail);
    example.size=2;
    example.head->next=example.tail;
    example.tail->next=NULL;
    example.tail->data=2;
    assert(back(&example)==2);
    assert(received==SIG_DEF);
}

void test_resize(){
    linked_list example={NULL,NULL,0};

    resize(&example,-1);
    assert(received==SIG_TRN);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    resize(&example,2);
    assert(received==SIG_DEF);
    assert(example.head->next==example.tail);
    assert(example.head->data==0);
    assert(example.tail->data==0);
    assert(example.size==2);

    resize(&example,3);
    assert(received==SIG_DEF);
    assert(example.head->next->next==example.tail);
    assert(example.head->data==0);
    assert(example.head->next->data==0);
    assert(example.tail->data==0);
    assert(example.size==3);

    resize(&example,1);
    assert(received==SIG_DEF);
    assert(example.head==example.tail);
    assert(example.size==1);
    assert(example.head->data==0);
    assert(example.tail->next==NULL);

    resize(&example,0);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

void test_size(){
    linked_list example={calloc(1,sizeof(node)),calloc(1,sizeof(node)),2};
    assert(example.head);
    assert(example.tail);

    example.head->next=example.tail;
    assert(size(&example)==2);
    assert(received==SIG_DEF);

    free(example.tail);
    free(example.head);
    example.size=0;

    assert(size(&example)==0);
    assert(received==SIG_DEF);
}

void test_clear(){
    linked_list example={malloc(sizeof(node)),calloc(1,sizeof(node)),3};
    assert(example.head);
    assert(example.tail);

    node* sec=malloc(sizeof(node));
    assert(sec);
    example.head->next=sec;
    sec->next=example.tail;

    clear(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);

    clear(&example);
    assert(received==SIG_DEF);
    assert(example.head==NULL);
    assert(example.tail==NULL);
    assert(example.size==0);
}

#endif

int main(void)
{
    linked_list ex;

    ex.initialize();
    assert(ex.size() == 0);
    ex.push_back(3);
    assert(ex.size() == 1);
    ex.pop_back();
    assert(ex.size() == 0);

    #ifndef DUMMY_MACRO
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
    #endif

    return 0;
}