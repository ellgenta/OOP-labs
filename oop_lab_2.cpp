#include <iostream>

using color = enum {red, black};

class node {
public:
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

int main(void) {

    return 0;
}