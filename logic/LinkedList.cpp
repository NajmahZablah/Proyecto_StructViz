#include "LinkedList.h"
#include <iostream>

// CONSTRUCTOR / DESTRUCTOR
LinkedList::LinkedList() :
    head(nullptr), _size(0) {}

LinkedList::~LinkedList() {
    clear();
}

// INSERCIÓN
// Crea un nodo y lo coloca al frente
void LinkedList::insertAtHead(int val) {
    Node* newNode = new Node(val);
    newNode->next = head;
    head = newNode;
    _size++;
}

// Recorre hasta el último nodo y enlaza el nuevo al final
void LinkedList::insertAtTail(int val) {
    Node* newNode = new Node(val);

    if (!head) {
        head = newNode;
        _size++;
        return;
    }

    Node* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
    _size++;
}

// Inserta en una posición arbitraria
void LinkedList::insertAt(int index, int val) {
    if (index <= 0) {
        insertAtHead(val);
        return;
    }

    if (index >= _size) {
        insertAtTail(val);
        return;
    }

    Node* newNode = new Node(val);
    Node* curr = head;
    for (int i = 0; i < index - 1; i++) curr = curr->next;
    newNode->next = curr->next;
    curr->next = newNode;
    _size++;
}

// ELIMINACIÓN
// Busca el valor y reelanza los punteros vecinos
bool LinkedList::remove(int val) {
    if (!head) {
        return false;
    }

    if (head->data == val) {
        Node* temp = head;
        head = head->next;
        delete temp;
        _size--;
        return true;
    }

    Node* curr = head;
    while (curr->next && curr->next->data != val) {
        curr = curr->next;
    }

    if (!curr->next) {
        return false;
    }

    Node* temp = curr->next;
    curr->next = temp->next;
    delete temp;
    _size--;
    return true;
}

// BÚSQUEDA
// Recorrido lineal O(n)
bool LinkedList::search(int val) {
    Node* curr = head;

    while (curr) {
        if (curr->data == val) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// UTILIDADES
// Imprime la lista en consola
void LinkedList::traverse() {
    if (!head) {
        std::cout << "nullptr (lista vacia)" << std::endl;
        return;
    }

    Node* curr = head;
    std::cout << "head -> ";

    while (curr) {
        std::cout << "[" << curr->data << "]";
        if (curr->next) std::cout << " -> ";
        curr = curr->next;
    }
    std::cout << " -> nullptr" << std::endl;
}

// Libera cada nodo uno a uno y reinicia el contador
void LinkedList::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    _size = 0;
}

// ACCESORES
Node* LinkedList::getHead() const {
    return head;
}

int LinkedList::size() const {
    return _size;
}
