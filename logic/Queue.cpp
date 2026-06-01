#include "Queue.h"
#include <iostream>

// CONSTRUCTOR / DESTRUCTOR
Queue::Queue() : front(nullptr), back(nullptr), _size(0) {}

Queue::~Queue() {
    clear();
}

// ENQUEUE
// Crea un nodo y lo enlaza al final (back)
void Queue::enqueue(int val) {
    Node* newNode = new Node(val);

    // 1er caso: cola vacía
    if (isEmpty()) {
        front = newNode;
        back = newNode;
        _size++;
        return;
    }

    // 2do caso: cola NO vacía
    back->next = newNode;
    back = newNode;
    _size++;
}

// DEQUEUE
// Elimina el front y retorna su valor
int Queue::dequeue() {
    if (isEmpty()) {
        std::cerr << "dequeue() llamado en queue vacía" << std::endl;
        return -1;
    }

    Node* temp = front;
    int returned = temp->value;
    front = front->next;

    if (front == nullptr) {
        back = nullptr;
    }

    delete temp;
    _size--;
    return returned;
}

// PEEK
// Retorna el valor del front sin modificar la cola
int Queue::peek() const {
    if (isEmpty()) {
        std::cerr << "peek() llamado en queue vacía" << std::endl;
        return -1;
    }
    return front->value;
}

// REAR
// Retorna el valor del back sin modificar la cola
int Queue::rear() const {
    if (isEmpty()) {
        std::cerr << "rear() llamado en queue vacía" << std::endl;
        return -1;
    }
    return back->value;
}

// ISEMPTY
bool Queue::isEmpty() const {
    return front == nullptr;
}

// SIZE
int Queue::size() const {
    return _size;
}

// CLEAR
// Libera todos los nodos usando dequeue()
void Queue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}

// ACCESORES
Node* Queue::getFront() const {
    return front;
}

Node* Queue::getBack() const {
    return back;
}
