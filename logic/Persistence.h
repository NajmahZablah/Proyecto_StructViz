#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "DoubleLinkedList.h"
#include "BST.h"
#include "AVL.h"

// PERSISTENCE
// Guarda y carga todas las estructuras en un archivo binario
class Persistence {
public:
    static bool save(const std::string& filename,
                     LinkedList* ll,
                     Stack* stack,
                     Queue* queue,
                     DoubleLinkedList* dll,
                     BST* bst,
                     AVL* avl);

    static bool load(const std::string& filename,
                     LinkedList* ll,
                     Stack* stack,
                     Queue* queue,
                     DoubleLinkedList* dll,
                     BST* bst,
                     AVL* avl);

private:
    // Qué estructura sigue en el archivo?
    static const int SECTION_LL = 1;
    static const int SECTION_STACK = 2;
    static const int SECTION_QUEUE = 3;
    static const int SECTION_DLL = 4;
    static const int SECTION_BST = 5;
    static const int SECTION_AVL = 6;

    // Valor de bandera
    static const int SENTINEL = -2147483648; // INT_MIN

    // Tope para arreglos temporales de carga (mismo límite usado
    // en MainWindow para recorridos: suficiente para este proyecto)
    static const int MAX_NODES = 1024;

    // Helpers para escribir y leer
    static void writeInt(std::ofstream& file, int val);
    static bool readInt(std::ifstream& file, int& val);

    // Helpers para guardar recorridos de árboles
    static void saveBSTPreorder(std::ofstream& file, BSTNode* node);
    static void saveAVLPreorder(std::ofstream& file, AVLNode* node);
};

#endif // PERSISTENCE_H
