#ifndef AVL_H
#define AVL_H

// NODO DEL AVL: guarda su propia altura
struct AVLNode {
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int val) : value(val), height(1), left(nullptr), right(nullptr) {}
};

// ÁRBOL AVL
class AVL {
public:
    AVL();
    ~AVL();

    // Operaciones
    void insert(int val);
    bool search(int val) const;
    void remove(int val);

    // Recorridos
    void inorder(AVLNode* node, int* arr, int& idx) const;
    void preorder(AVLNode* node, int* arr, int& idx) const;
    void postorder(AVLNode* node, int* arr, int& idx) const;

    // Data por nodo
    int getHeight(AVLNode* node) const;
    int getBalance(AVLNode* node) const;
    int depth(AVLNode* node, int val, int currentDepth = 0) const;

    // Predecesor y sucesor
    AVLNode* successor(AVLNode* node) const;
    AVLNode* predecessor(AVLNode* node) const;

    // Utilidades
    void clear();
    bool isEmpty() const;
    int size() const;

    // Accesor
    AVLNode* getRoot() const;

private:
    AVLNode* root;
    int _size;

    // Funciones recursivas
    AVLNode* insertRecursive(AVLNode* currentRoot, AVLNode* newNode);
    AVLNode* searchRecursive(AVLNode* currentRoot, int val) const;
    AVLNode* removeRecursive(AVLNode* currentRoot, int val);
    AVLNode* clearRecursive(AVLNode* currentRoot);
    AVLNode* mostLeftChild(AVLNode* currentRoot) const;
    AVLNode* mostRightChild(AVLNode* currentRoot) const;

    // Rotaciones
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* rightRotate(AVLNode* y);

    // Helper
    int maxVal(int a, int b) const;
};

#endif // AVL_H
