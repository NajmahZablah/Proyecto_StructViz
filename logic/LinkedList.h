#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// NODO: unidad mínima de la lista
struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

// LISTA ENLAZADA SIMPLE
class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    // Inserción
    void insertAtHead(int val); // O(1)
    void insertAtTail(int val); // 0(n)
    void insertAt(int index, int val); // O(n)

    // Eliminación
    bool remove(int val);

    // Búsqueda
    bool search(int val);

    // Utilidades
    void traverse();
    void clear();

    // Accesores
    Node* getHead() const;
    int size() const;

private:
    Node* head;
    int _size;
};

#endif // LINKEDLIST_H
