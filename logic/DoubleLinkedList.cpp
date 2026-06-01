#include "DoubleLinkedList.h"
#include <iostream>

// CONSTRUCTOR / DESTRUCTOR
DoubleLinkedList::DoubleLinkedList() : head(nullptr), tail(nullptr), _size(0) {}

DoubleLinkedList::~DoubleLinkedList() {
    clear();
}

// INSERTATHEAD
// Crea un nodo y lo coloca al frente
void DoubleLinkedList::insertAtHead(int val) {
    DNode* newNode = new DNode(val);

    // 1er caso: lista vacía
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
        _size++;
        return;
    }

    // 2do caso: lista NO vacía
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
    _size++;
}

// INSERTATTAIL
// Crea un nodo y lo enlaza al final
void DoubleLinkedList::insertAtTail(int val) {
    DNode* newNode = new DNode(val);

    // 1er caso: lista vacía
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
        _size++;
        return;
    }

    // 2do caso: lista NO vacía
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    _size++;
}

// INSERTAT
// Inserta en una posición arbitraria
void DoubleLinkedList::insertAt(int index, int val) {
    if (index <= 0) {
        insertAtHead(val);
        return;
    }
    if (index >= _size) {
        insertAtTail(val);
        return;
    }

    DNode* curr = head;
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }

    DNode* newNode = new DNode(val);
    DNode* prevNode = curr->prev;

    newNode->next = curr;
    newNode->prev = prevNode;
    prevNode->next = newNode;
    curr->prev = newNode;
    _size++;
}

// REMOVEBYREFERENCE
// Elimina un nodo dado su puntero directo
void DoubleLinkedList::removeByReference(DNode* ref) {
    if (ref == nullptr) {
        return;
    }

    // 1er caso: es el head
    if (ref == head) {
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }

        // 2do caso: es el tail
    } else if (ref == tail) {
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        }

        // 3er caso: nodo intermedio
    } else {
        ref->prev->next = ref->next;
        ref->next->prev = ref->prev;
    }

    delete ref;
    _size--;
}

// REMOVE
// Busca por valor y elimina usando removeByReference
bool DoubleLinkedList::remove(int val) {
    DNode* curr = head;
    while (curr) {
        if (curr->value == val) {
            removeByReference(curr);
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// SEARCH
// Recorrido lineal
bool DoubleLinkedList::search(int val) const {
    DNode* curr = head;
    while (curr) {
        if (curr->value == val) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// CLEAR
// Libera todos los nodos uno a uno
void DoubleLinkedList::clear() {
    while (head) {
        DNode* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    _size = 0;
}

// UTILIDADES
bool DoubleLinkedList::isEmpty() const {
    return head == nullptr;
}

int DoubleLinkedList::size() const {
    return _size;
}

// ACCESORES
DNode* DoubleLinkedList::getHead() const {
    return head;
}

DNode* DoubleLinkedList::getTail() const {
    return tail;
}
