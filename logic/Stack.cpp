#include "Stack.h"
#include <iostream>

// CONSTRUCTOR / DESTRUCTOR
Stack::Stack() : top(nullptr), _size(0) {}

Stack::~Stack() {
    clear();
}

// PUSH
// Crea un nodo nuevo y lo coloca en el top
void Stack::push(int val) {
    Node* newNode = new Node(val);
    newNode->next = top;
    top = newNode;
    _size++;
}

// POP
// Elimina el top y retorna su valor
int Stack::pop() {
    if (isEmpty()) {
        std::cerr << "pop() llamado en stack vacío" << std::endl;
        return -1;
    }
    Node* temp = top;
    int returned = temp->value;
    top = top->next;
    delete temp;
    _size--;
    return returned;
}

// PEEK
// Retorna el valor del top sin modificar la pila
int Stack::peek() const {
    if (isEmpty()) {
        std::cerr << "peek() llamado en stack vacío" << std::endl;
        return -1;
    }
    return top->value;
}

// ISEMPTY
bool Stack::isEmpty() const {
    return top == nullptr;
}

// SIZE
int Stack::size() const {
    return _size;
}

// CLEAR
// Libera todos los nodos usando pop()
void Stack::clear() {
    while (!isEmpty()) {
        pop();
    }
}

// GETTOP
// Accesor para que el widget pueda recorrer los nodos
Node* Stack::getTop() const {
    return top;
}
