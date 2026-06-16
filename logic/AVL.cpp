#include "AVL.h"

// CONSTRUCTOR / DESTRUCTOR
AVL::AVL() : root(nullptr), _size(0) {}

AVL::~AVL() {
    clear();
}

// HELPERS
int AVL::maxVal(int a, int b) const {
    return (a > b) ? a : b;
}

// Altura almacenada en el nodo
int AVL::getHeight(AVLNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Balance = altura izq - altura der
// AVL válido: entre -1 y 1
int AVL::getBalance(AVLNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

int AVL::depth(AVLNode* node, int val, int currentDepth) const {
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

AVLNode* AVL::mostLeftChild(AVLNode* currentRoot) const {
    while (currentRoot->left != nullptr) {
        currentRoot = currentRoot->left;
    }
    return currentRoot;
}

AVLNode* AVL::mostRightChild(AVLNode* currentRoot) const {
    while (currentRoot->right != nullptr) {
        currentRoot = currentRoot->right;
    }
    return currentRoot;
}

// SUCESOR
AVLNode* AVL::successor(AVLNode* node) const {
    return mostLeftChild(node->right);
}

// PREDECESOR
AVLNode* AVL::predecessor(AVLNode* node) const {
    return mostRightChild(node->left);
}

// ROTACIONES
AVLNode* AVL::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // reasignar punteros
    x->right = T2;
    y->left = x;

    // actualizar alturas
    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLNode* AVL::rightRotate(AVLNode* y) {
    AVLNode* x  = y->left;
    AVLNode* T2 = x->right;

    // reasignar punteros
    y->left  = T2;
    x->right = y;

    // actualizar alturas
    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// INSERT
void AVL::insert(int val) {
    AVLNode* newNode = new AVLNode(val);

    // 1er caso: árbol vacío
    if (root == nullptr) {
        root = newNode;
        _size++;
        return;
    }

    // 2do caso: árbol no vacío
    root = insertRecursive(root, newNode);
}

AVLNode* AVL::insertRecursive(AVLNode* currentRoot, AVLNode* newNode) {
    if (currentRoot == nullptr) {
        _size++;
        return newNode;
    }

    if (newNode->value < currentRoot->value) {
        currentRoot->left = insertRecursive(currentRoot->left, newNode);
    } else if (newNode->value > currentRoot->value) {
        currentRoot->right = insertRecursive(currentRoot->right, newNode);
    } else {
        delete newNode;
        return currentRoot;
    }

    currentRoot->height = maxVal(getHeight(currentRoot->left),
                                 getHeight(currentRoot->right)) + 1;

    int balance = getBalance(currentRoot);

    // Caso 1: Left Left
    // desbalance izquierdo, el nuevo nodo cayó en el subárbol izquierdo-izquierdo
    if (balance > 1 && newNode->value < currentRoot->left->value) {
        return rightRotate(currentRoot);
    }

    // Caso 2: Right Right
    // desbalance derecho, el nuevo nodo cayó en el subárbol derecho-derecho
    if (balance < -1 && newNode->value > currentRoot->right->value) {
        return leftRotate(currentRoot);
    }

    // Caso 3: Left Right
    // desbalance izquierdo, pero el nuevo nodo cayó en el subárbol izquierdo-derecho
    if (balance > 1 && newNode->value > currentRoot->left->value) {
        currentRoot->left = leftRotate(currentRoot->left);
        return rightRotate(currentRoot);
    }

    // Caso 4: Right Left
    // desbalance derecho, pero el nuevo nodo cayó en el subárbol derecho-izquierdo
    if (balance < -1 && newNode->value < currentRoot->right->value) {
        currentRoot->right = rightRotate(currentRoot->right);
        return leftRotate(currentRoot);
    }

    return currentRoot;
}

// SEARCH
bool AVL::search(int val) const {
    return searchRecursive(root, val) != nullptr;
}

AVLNode* AVL::searchRecursive(AVLNode* currentRoot, int val) const {
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

// REMOVE
void AVL::remove(int val) {
    root = removeRecursive(root, val);
}

AVLNode* AVL::removeRecursive(AVLNode* currentRoot, int val) {
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
            AVLNode* nodeToAscend = currentRoot->left;
            delete currentRoot;
            _size--;
            return nodeToAscend;
        }

        // 3er caso: solo tiene hijo derecho
        else if (currentRoot->left == nullptr) {
            AVLNode* nodeToAscend = currentRoot->right;
            delete currentRoot;
            _size--;
            return nodeToAscend;
        }

        // 4to caso: tiene dos hijos
        else {
            AVLNode* succ = successor(currentRoot);
            currentRoot->value = succ->value;
            currentRoot->right = removeRecursive(currentRoot->right, succ->value);
        }
    }

    // REBALANCEO
    currentRoot->height = maxVal(getHeight(currentRoot->left),
                                 getHeight(currentRoot->right)) + 1;

    int balance = getBalance(currentRoot);

    // Left Left
    if (balance > 1 && getBalance(currentRoot->left) >= 0) {
        return rightRotate(currentRoot);
    }

    // Left Right
    if (balance > 1 && getBalance(currentRoot->left) < 0) {
        currentRoot->left = leftRotate(currentRoot->left);
        return rightRotate(currentRoot);
    }

    // Right Right
    if (balance < -1 && getBalance(currentRoot->right) <= 0) {
        return leftRotate(currentRoot);
    }

    // Right Left
    if (balance < -1 && getBalance(currentRoot->right) > 0) {
        currentRoot->right = rightRotate(currentRoot->right);
        return leftRotate(currentRoot);
    }

    return currentRoot;
}

// RECORRIDOS
void AVL::inorder(AVLNode* node, int* arr, int& idx) const {
    if (node == nullptr) {
        return;
    }
    inorder(node->left, arr, idx);
    arr[idx++] = node->value;
    inorder(node->right, arr, idx);
}

void AVL::preorder(AVLNode* node, int* arr, int& idx) const {
    if (node == nullptr) {
        return;
    }
    arr[idx++] = node->value;
    preorder(node->left, arr, idx);
    preorder(node->right, arr, idx);
}

void AVL::postorder(AVLNode* node, int* arr, int& idx) const {
    if (node == nullptr) {
        return;
    }
    postorder(node->left, arr, idx);
    postorder(node->right, arr, idx);
    arr[idx++] = node->value;
}

// CLEAR + UTILIDADES
AVLNode* AVL::clearRecursive(AVLNode* currentRoot) {
    if (currentRoot == nullptr) {
        return nullptr;
    }
    currentRoot->left  = clearRecursive(currentRoot->left);
    currentRoot->right = clearRecursive(currentRoot->right);
    delete currentRoot;
    return nullptr;
}

void AVL::clear() {
    root = clearRecursive(root);
    _size = 0;
}

bool AVL::isEmpty() const {
    return root == nullptr;
}

int AVL::size() const {
    return _size;
}

AVLNode* AVL::getRoot() const {
    return root;
}
