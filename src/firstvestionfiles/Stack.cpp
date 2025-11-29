#include "firstvertionfiles/Stack.hpp"

Stack::Stack() : top(nullptr) {}

Stack::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

void Stack::push(char bracket, int lineNumber, int position) {
    StackNode* newNode = new StackNode;
    newNode->bracket = bracket;
    newNode->lineNumber = lineNumber;
    newNode->position = position;
    newNode->next = top;
    top = newNode;
}

void Stack::pop() {
    if (isEmpty()) return;

    StackNode* temp = top;
    top = top->next;
    delete temp;
}

StackNode* Stack::peek() const {
    return top;
}

bool Stack::isEmpty() const {
    return top == nullptr;
}