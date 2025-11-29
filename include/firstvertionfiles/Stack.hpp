#ifndef STACK_HPP
#define STACK_HPP

struct StackNode {
    char bracket;
    int lineNumber;
    int position;
    StackNode* next;
};

class Stack {
private:
    StackNode* top;

public:
    Stack();
    ~Stack();

    void push(char bracket, int lineNumber, int position);
    void pop();
    StackNode* peek() const;
    bool isEmpty() const;
};

#endif