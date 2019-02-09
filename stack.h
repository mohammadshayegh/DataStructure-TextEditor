#ifndef STACK_H
#define STACK_H
#include <iostream>
#include <string.h>
using namespace std ;
struct node
{
    char val ;
    node* next ;
    node* prev ;
};

struct stack{
    node* pos ;
    stack* next ;
};

class Stack
{
public:
    stack* first = NULL ;
public:
    void Push(node* pos);
    node* Pop();
    void print();
    void clean();
};

#endif // STACK_H
