#include "Persistence.h"
#include <fstream>
#include <iostream>
#include <vector>

// HELPERS BINARIOS
// Escribe un int como 4 bytes en el archivo
void Persistence::writeInt(std::ofstream& file, int val) {
    file.write(reinterpret_cast<const char*>(&val), sizeof(int));
}

// Lee 4 bytes del archivo y los interpreta como int
bool Persistence::readInt(std::ifstream& file, int& val) {
    file.read(reinterpret_cast<char*>(&val), sizeof(int));
    return file.gcount() == sizeof(int);
}

// HELPERS DE ÁRBOLES
void Persistence::saveBSTPreorder(std::ofstream& file, BSTNode* node) {
    if (node == nullptr) {
        return;
    }
    writeInt(file, node->value);
    saveBSTPreorder(file, node->left);
    saveBSTPreorder(file, node->right);
}

void Persistence::saveAVLPreorder(std::ofstream& file, AVLNode* node) {
    if (node == nullptr) {
        return;
    }
    writeInt(file, node->value);
    saveAVLPreorder(file, node->left);
    saveAVLPreorder(file, node->right);
}

// SAVE
// Estructura del archivo:
//   [código sección] [val1] [val2] ... [SENTINEL]
bool Persistence::save(const std::string& filename,
                       LinkedList* ll,
                       Stack* stack,
                       Queue* queue,
                       DoubleLinkedList* dll,
                       BST* bst,
                       AVL* avl)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para guardar: " << filename << std::endl;
        return false;
    }

    // LISTA ENLAZADA
    writeInt(file, SECTION_LL);
    Node* currLL = ll->getHead();
    while (currLL) {
        writeInt(file, currLL->value);
        currLL = currLL->next;
    }
    writeInt(file, SENTINEL);

    // STACK
    writeInt(file, SECTION_STACK);
    Node* currSt = stack->getTop();
    while (currSt) {
        writeInt(file, currSt->value);
        currSt = currSt->next;
    }
    writeInt(file, SENTINEL);

    // QUEUE
    writeInt(file, SECTION_QUEUE);
    Node* currQ = queue->getFront();
    while (currQ) {
        writeInt(file, currQ->value);
        currQ = currQ->next;
    }
    writeInt(file, SENTINEL);

    // DOUBLE LINKED LIST
    writeInt(file, SECTION_DLL);
    DNode* currDLL = dll->getHead();
    while (currDLL) {
        writeInt(file, currDLL->value);
        currDLL = currDLL->next;
    }
    writeInt(file, SENTINEL);

    // BST
    writeInt(file, SECTION_BST);
    saveBSTPreorder(file, bst->getRoot());
    writeInt(file, SENTINEL);

    // AVL
    writeInt(file, SECTION_AVL);
    saveAVLPreorder(file, avl->getRoot());
    writeInt(file, SENTINEL);

    file.close();
    return true;
}

// LOAD
// Lee sección por sección hasta el SENTINEL
// luego inserta los valores en la estructura correspondiente
bool Persistence::load(const std::string& filename,
                       LinkedList* ll,
                       Stack* stack,
                       Queue* queue,
                       DoubleLinkedList* dll,
                       BST* bst,
                       AVL* avl)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para cargar: " << filename << std::endl;
        return false;
    }

    // Limpiar todas las estructuras antes de cargar
    ll->clear();
    stack->clear();
    queue->clear();
    dll->clear();
    bst->clear();
    avl->clear();

    int val;

    // Leer sección por sección
    while (readInt(file, val)) {
        int section = val;

        if (section == SECTION_LL) {
            while (readInt(file, val) && val != SENTINEL) {
                ll->insertAtTail(val);
            }
        }

        else if (section == SECTION_STACK) {
            std::vector<int> vals;
            while (readInt(file, val) && val != SENTINEL) {
                vals.push_back(val);
            }
            for (int i = (int)vals.size() - 1; i >= 0; i--) {
                stack->push(vals[i]);
            }
        }

        else if (section == SECTION_QUEUE) {
            while (readInt(file, val) && val != SENTINEL) {
                queue->enqueue(val);
            }
        }

        else if (section == SECTION_DLL) {
            while (readInt(file, val) && val != SENTINEL) {
                dll->insertAtTail(val);
            }
        }

        else if (section == SECTION_BST) {
            while (readInt(file, val) && val != SENTINEL) {
                bst->insert(val);
            }
        }

        else if (section == SECTION_AVL) {
            while (readInt(file, val) && val != SENTINEL) {
                avl->insert(val);
            }
        }
    }

    file.close();
    return true;
}
