#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "DoubleLinkedList.h"

// PERSISTENCE
// Guarda y carga el estado de todas las estructuras en un archivo de texto
class Persistence {
public:
    static bool save(const std::string& filename,
                     LinkedList* ll,
                     Stack* stack,
                     Queue* queue,
                     DoubleLinkedList* dll);

    static bool load(const std::string& filename,
                     LinkedList* ll,
                     Stack* stack,
                     Queue* queue,
                     DoubleLinkedList* dll);
};

#endif // PERSISTENCE_H
