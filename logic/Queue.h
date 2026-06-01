#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h" // struct Node

// QUEUE (Cola) - Estructura FIFO
// Se inserta por el back y se elimina por el front
class Queue {
public:
    Queue();
    ~Queue();

    // Operaciones principales
    void enqueue(int val); // O(1)
    int dequeue(); // O(1)
    int  peek() const; // O(1)
    int  rear() const; // O(1)

    // Utilidades
    bool isEmpty() const;
    int size() const;
    void clear();

    // Accesores para el widget
    Node* getFront() const;
    Node* getBack() const;

private:
    Node* front;
    Node* back;
    int _size;
};

#endif // QUEUE_H
