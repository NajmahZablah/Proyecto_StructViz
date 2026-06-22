#include "BST.h"

// CONSTRUCTOR / DESTRUCTOR
BST::BST() : root(nullptr), _size(0) {}

BST::~BST() {
    clear();
}

// INSERT
void BST::insert(int val) {
    BSTNode* newNode = new BSTNode(val);

    // 1er caso: árbol vacío
    if (root == nullptr) {
        root = newNode;
        _size++;
        return;
    }

    // 2do caso: árbol no vacío
    root = insertRecursive(root, newNode);
}

BSTNode* BST::insertRecursive(BSTNode* currentRoot, BSTNode* newNode) {
    if (currentRoot == nullptr) {
        _size++;
        return newNode;
    }

    if (newNode->value < currentRoot->value) {
        currentRoot->left = insertRecursive(currentRoot->left, newNode);
    } else if (newNode->value > currentRoot->value) {
        currentRoot->right = insertRecursive(currentRoot->right, newNode);
    } else {
        // Valor duplicado: no se inserta. Liberamos newNode para evitar
        // una fuga de memoria, ya que quedaría sin padre que lo enlace.
        delete newNode;
    }

    return currentRoot;
}

// SEARCH
bool BST::search(int val) {
    BSTNode* result = searchRecursive(root, val);
    return result != nullptr;
}

BSTNode* BST::searchRecursive(BSTNode* currentRoot, int val) const {
    // caso base: no lo encontré
    if (currentRoot == nullptr) {
        return nullptr;
    }

    if (currentRoot->value == val) {
        return currentRoot;
    }

    if (val > currentRoot->value) {
        return searchRecursive(currentRoot->right, val);
    } else {
        return searchRecursive(currentRoot->left, val);
    }
}

// HELPERS
BSTNode* BST::mostLeftChild(BSTNode* currentRoot) const {
    while (currentRoot->left != nullptr) {
        currentRoot = currentRoot->left;
    }
    return currentRoot;
}

BSTNode* BST::mostRightChild(BSTNode* currentRoot) const {
    while (currentRoot->right != nullptr) {
        currentRoot = currentRoot->right;
    }
    return currentRoot;
}

// SUCESOR
BSTNode* BST::successor(BSTNode* node) const {
    return mostLeftChild(node->right);
}

// PREDECESOR
BSTNode* BST::predecessor(BSTNode* node) const {
    return mostRightChild(node->left);
}

// REMOVE
void BST::remove(int val) {
    root = removeRecursive(root, val);
}

BSTNode* BST::removeRecursive(BSTNode* currentRoot, int val) {
    if (currentRoot == nullptr) {
        return nullptr;
    }

    if (val < currentRoot->value) {
        currentRoot->left = removeRecursive(currentRoot->left, val);
    } else if (val > currentRoot->value) {
        currentRoot->right = removeRecursive(currentRoot->right, val);
    } else {

        // 1er caso: no tiene hijos
        if (currentRoot->left == nullptr && currentRoot->right == nullptr) {
            delete currentRoot;
            _size--;
            return nullptr;
        }

        // 2do caso: solo tiene hijo izquierdo
        else if (currentRoot->right == nullptr) {
            BSTNode* nodeToAscend = currentRoot->left;
            delete currentRoot;
            _size--;
            return nodeToAscend;
        }

        // 3er caso: solo tiene hijo derecho
        else if (currentRoot->left == nullptr) {
            BSTNode* nodeToAscend = currentRoot->right;
            delete currentRoot;
            _size--;
            return nodeToAscend;
        }

        // 4to caso: tiene dos hijos
        else {
            BSTNode* succ = successor(currentRoot);
            currentRoot->value = succ->value;
            currentRoot->right = removeRecursive(currentRoot->right, succ->value);
        }
    }

    return currentRoot;
}

// RECORRIDOS
void BST::inorder(BSTNode* node, int* arr, int& idx) const {
    if (node == nullptr) {
        return;
    }
    inorder(node->left, arr, idx);
    arr[idx++] = node->value;
    inorder(node->right, arr, idx);
}

void BST::preorder(BSTNode* node, int* arr, int& idx) const {
    if (node == nullptr) {
        return;
    }
    arr[idx++] = node->value;
    preorder(node->left, arr, idx);
    preorder(node->right, arr, idx);
}

void BST::postorder(BSTNode* node, int* arr, int& idx) const {
    if (node == nullptr) {
        return;
    }
    postorder(node->left, arr, idx);
    postorder(node->right, arr, idx);
    arr[idx++] = node->value;
}

// HEIGHT
// longitud del camino más largo hacia una hoja
int BST::height(BSTNode* node) const {
    if (node == nullptr) {
        return -1;
    }

    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    }
    return rightHeight + 1;
}

// DEPTH
// distancia desde la raíz hasta el nodo con ese valor
int BST::depth(BSTNode* node, int val, int currentDepth) const {
    if (node == nullptr) {
        return -1;
    }

    if (node->value == val) {
        return currentDepth;
    }

    if (val < node->value) {
        return depth(node->left, val, currentDepth + 1);
    } else {
        return depth(node->right, val, currentDepth + 1);
    }
}

// BALANCE FACTOR: altura(izq) - altura(der)
int BST::balanceFactor(BSTNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// CLEAR
BSTNode* BST::clearRecursive(BSTNode* currentRoot) {
    if (currentRoot == nullptr) {
        return nullptr;
    }
    currentRoot->left = clearRecursive(currentRoot->left);
    currentRoot->right = clearRecursive(currentRoot->right);
    delete currentRoot;
    return nullptr;
}

void BST::clear() {
    root = clearRecursive(root);
    _size = 0;
}

// UTILIDADES
bool BST::isEmpty() const {
    return root == nullptr;
}

int BST::size() const {
    return _size;
}

// ACCESOR
BSTNode* BST::getRoot() const {
    return root;
}
