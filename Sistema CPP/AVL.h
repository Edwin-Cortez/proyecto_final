#pragma once
#include "Student.h"

struct AVLNode {
    Student data;
    AVLNode* izquierda;
    AVLNode* derecha;
    int altura;

    AVLNode(const Student& s);
};

class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    void insert(const Student& s);
    void remove(int skill_score, int student_id);
    AVLNode* search(int skill_score) const;
    void printRanking(bool imprimir = true) const;

    int getHeight() const;
    int getRootBalanceFactor() const;
    int getNodeCount() const;

private:
    AVLNode* raiz;
    int totalNodos;

    int obtenerAltura(AVLNode* nodo) const;
    int obtenerBalance(AVLNode* nodo) const;
    AVLNode* rotarDerecha(AVLNode* y);
    AVLNode* rotarIzquierda(AVLNode* x);
    AVLNode* insertar(AVLNode* nodo, const Student& s);
    AVLNode* eliminar(AVLNode* nodo, int skill_score, int student_id);
    AVLNode* getMenor(AVLNode* nodo);
    AVLNode* buscar(AVLNode* nodo, int skill_score) const;
    void mostrarRanking(AVLNode* nodo, int& pos, bool imprimir) const;
    void destruir(AVLNode* nodo);
};_H