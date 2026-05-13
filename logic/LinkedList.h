#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void insertAtHead(int val);
    void insertAtTail(int val);
    void insertAt(int index, int val);
    bool remove(int val);
    bool search(int val);
    void traverse();
    void clear();

    Node* getHead() const;
    int size() const;

private:
    Node* head;
    int _size;
};

#endif // LINKEDLIST_H
