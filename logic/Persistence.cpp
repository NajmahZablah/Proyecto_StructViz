#include "Persistence.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// SAVE
bool Persistence::save(const std::string& filename,
                       LinkedList* ll,
                       Stack* stack,
                       Queue* queue,
                       DoubleLinkedList* dll)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para guardar: " << filename << std::endl;
        return false;
    }

    // LISTA ENLAZADA
    file << "LL\n";
    if (!ll->getHead()) {
        file << "null\n";
    } else {
        Node* curr = ll->getHead();
        while (curr) {
            file << curr->value;
            if (curr->next) {
                file << " -> ";
            }
            curr = curr->next;
        }
        file << " -> null\n";
    }

    // STACK
    file << "\nSTACK\n";
    if (stack->isEmpty()) {
        file << "null\n";
    } else {
        Node* curr = stack->getTop();
        while (curr) {
            file << curr->value;
            if (curr->next) {
                file << " -> ";
            }
            curr = curr->next;
        }
        file << " -> null\n";
    }

    // QUEUE
    file << "\nQUEUE\n";
    if (queue->isEmpty()) {
        file << "null\n";
    } else {
        Node* curr = queue->getFront();
        while (curr) {
            file << curr->value;
            if (curr->next) {
                file << " -> ";
            }
            curr = curr->next;
        }
        file << " -> null\n";
    }

    // LISTA DOBLEMENTE ENLAZADA
    file << "\nDLL\n";
    if (dll->isEmpty()) {
        file << "null\n";
    } else {
        file << "null <- ";
        DNode* curr = dll->getHead();
        while (curr) {
            file << curr->value;
            if (curr->next) {
                file << " <-> ";
            }
            curr = curr->next;
        }
        file << " -> null\n";
    }

    file.close();
    return true;
}

// LOAD
bool Persistence::load(const std::string& filename,
                       LinkedList* ll,
                       Stack* stack,
                       Queue* queue,
                       DoubleLinkedList* dll)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para cargar: " << filename << std::endl;
        return false;
    }

    ll->clear();
    stack->clear();
    queue->clear();
    dll->clear();

    std::string line;
    std::string section = "";

    while (std::getline(file, line)) {
        if (line == "LL") {
            section = "LL";
            continue;
        }

        if (line == "STACK") {
            section = "STACK";
            continue;
        }

        if (line == "QUEUE") {
            section = "QUEUE";
            continue;
        }

        if (line == "DLL") {
            section = "DLL";
            continue;
        }

        if (line.empty()) {
            continue;
        }

        if (line == "null") {
            continue;
        }

        std::string clean = line;

        if (clean.substr(0, 8) == "null <- ") {
            clean = clean.substr(8);
        }

        std::string suffix = " -> null";
        if (clean.size() >= suffix.size() &&
            clean.substr(clean.size() - suffix.size()) == suffix) {
            clean = clean.substr(0, clean.size() - suffix.size());
        }

        std::string sep = (section == "DLL") ? " <-> " : " -> ";
        std::string result = "";
        size_t pos = 0;
        while ((pos = clean.find(sep)) != std::string::npos) {
            result += clean.substr(0, pos) + " ";
            clean = clean.substr(pos + sep.size());
        }
        result += clean;

        std::istringstream ss(result);
        int val;

        if (section == "LL") {
            while (ss >> val) {
                ll->insertAtTail(val);
            }
        } else if (section == "STACK") {
            std::vector<int> vals;
            while (ss >> val) {
                vals.push_back(val);
            }
            for (int i = (int)vals.size() - 1; i >= 0; i--) {
                stack->push(vals[i]);
            }
        } else if (section == "QUEUE") {
            while (ss >> val) {
                queue->enqueue(val);
            }
        } else if (section == "DLL") {
            while (ss >> val) {
                dll->insertAtTail(val);
            }
        }
    }

    file.close();
    return true;
}
