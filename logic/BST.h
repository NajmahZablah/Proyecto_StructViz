#ifndef BST_H
#define BST_H

// NODO DEL BST
struct BSTNode {
    int value;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

// ÁRBOL BINARIO DE BÚSQUEDA
class BST {
public:
    BST();
    ~BST();

    // Operaciones
    void insert(int val);
    bool search(int val);
    void remove(int val);

    // Recorridos
    void inorder(BSTNode* node, int* arr, int& idx) const;
    void preorder(BSTNode* node, int* arr, int& idx) const;
    void postorder(BSTNode* node, int* arr, int& idx) const;

    // Data por nodo
    int height(BSTNode* node) const;
    int depth(BSTNode* node, int val, int currentDepth = 0) const;
    int balanceFactor(BSTNode* node) const;

    // Predecesor y sucesor
    BSTNode* successor(BSTNode* node) const;
    BSTNode* predecessor(BSTNode* node) const;

    // Utilidades
    void clear();
    bool isEmpty() const;
    int size() const;

    // Accesor
    BSTNode* getRoot() const;

private:
    BSTNode* root;
    int _size;

    // Funciones recursivas
    BSTNode* insertRecursive(BSTNode* currentRoot, BSTNode* newNode);
    BSTNode* searchRecursive(BSTNode* currentRoot, int val) const;
    BSTNode* removeRecursive(BSTNode* currentRoot, int val);
    BSTNode* clearRecursive(BSTNode* currentRoot);
    BSTNode* mostLeftChild(BSTNode* currentRoot) const;
    BSTNode* mostRightChild(BSTNode* currentRoot) const;
};
#endif // BST_H
