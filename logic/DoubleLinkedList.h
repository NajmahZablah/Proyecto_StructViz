#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

// DNODO: unidad mínima de la lista doblemente enlazada
// Tiene puntero al siguiente Y al anterior
struct DNode {
    int value;
    DNode* next;
    DNode* prev;
    DNode(int val) : value(val), next(nullptr), prev(nullptr) {}
};

// LISTA DOBLEMENTE ENLAZADA
// Cada nodo conoce a su vecino izquierdo (prev) y derecho (next)
class DoubleLinkedList {
public:
    DoubleLinkedList();
    ~DoubleLinkedList();

    // Inserción
    void insertAtHead(int val); // O(1)
    void insertAtTail(int val); // O(1)
    void insertAt(int index, int val); // O(n)

    // Eliminación
    bool remove(int val);
    void removeByReference(DNode* ref);

    // Búsqueda
    bool search(int val) const;

    // Utilidades
    void clear();
    int size() const;
    bool isEmpty() const;

    // Accesores para el widget
    DNode* getHead() const;
    DNode* getTail() const;

private:
    DNode* head;
    DNode* tail;
    int _size;
};

#endif // DOUBLYLINKEDLIST_H
