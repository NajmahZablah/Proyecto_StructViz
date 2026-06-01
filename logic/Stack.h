#ifndef STACK_H
#define STACK_H

#include "LinkedList.h" // struct Node

// STACK (Pila) - Estructura LIFO
// El head apunta siempre al top de la pila
class Stack {
public:
    Stack();
    ~Stack();

    // Operaciones principales
    void push(int val); // O(1)
    int pop(); // O(1)
    int peek() const; // O(1)

    // Utilidades
    bool isEmpty() const;
    int size() const;
    void clear();

    // Accesor para el widget
    Node* getTop() const;

private:
    Node* top;
    int _size;
};

#endif // STACK_H
