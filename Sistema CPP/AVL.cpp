#include "AVL.h"
#include <iostream>
using namespace std;

// funcion propia para sacar el mayor de dos numeros
static int maximo(int a, int b) {
    if (a > b) return a;
    return b;
}

AVLNode::AVLNode(const Student& s) {
    data = s;
    izquierda = nullptr;
    derecha = nullptr;
    altura = 1;
}

AVLTree::AVLTree() {
    raiz = nullptr;
    totalNodos = 0;
}

AVLTree::~AVLTree() {
    destruir(raiz);
}

void AVLTree::destruir(AVLNode* nodo) {
    if (nodo == nullptr) return;
    destruir(nodo->izquierda);
    destruir(nodo->derecha);
    delete nodo;
}

int AVLTree::obtenerAltura(AVLNode* nodo) const {
    if (nodo == nullptr) return 0;
    return nodo->altura;
}

int AVLTree::obtenerBalance(AVLNode* nodo) const {
    if (nodo == nullptr) return 0;
    return obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha);
}

AVLNode* AVLTree::rotarDerecha(AVLNode* y) {
    AVLNode* x = y->izquierda;
    AVLNode* temp = x->derecha;

    x->derecha = y;
    y->izquierda = temp;

    y->altura = maximo(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;
    x->altura = maximo(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;

    return x;
}

AVLNode* AVLTree::rotarIzquierda(AVLNode* x) {
    AVLNode* y = x->derecha;
    AVLNode* temp = y->izquierda;

    y->izquierda = x;
    x->derecha = temp;

    x->altura = maximo(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;
    y->altura = maximo(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;

    return y;
}

// compara dos estudiantes por skill_score, desempata por student_id
static bool menorQue(const Student& a, const Student& b) {
    if (a.skill_score != b.skill_score)
        return a.skill_score < b.skill_score;
    return a.student_id < b.student_id;
}

AVLNode* AVLTree::insertar(AVLNode* nodo, const Student& s) {
    if (nodo == nullptr) return new AVLNode(s);

    if (menorQue(s, nodo->data)) {
        nodo->izquierda = insertar(nodo->izquierda, s);
    } else {
        nodo->derecha = insertar(nodo->derecha, s);
    }

    nodo->altura = 1 + maximo(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));

    int balance = obtenerBalance(nodo);

    // Caso LL
    if (balance > 1 && menorQue(s, nodo->izquierda->data))
        return rotarDerecha(nodo);

    // Caso RR
    if (balance < -1 && !menorQue(s, nodo->derecha->data))
        return rotarIzquierda(nodo);

    // Caso LR
    if (balance > 1 && !menorQue(s, nodo->izquierda->data)) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }

    // Caso RL
    if (balance < -1 && menorQue(s, nodo->derecha->data)) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void AVLTree::insert(const Student& s) {
    raiz = insertar(raiz, s);
    totalNodos++;
}

AVLNode* AVLTree::buscar(AVLNode* nodo, int skill_score) const {
    if (nodo == nullptr) return nullptr;
    if (skill_score == nodo->data.skill_score) return nodo;
    if (skill_score < nodo->data.skill_score) return buscar(nodo->izquierda, skill_score);
    return buscar(nodo->derecha, skill_score);
}

AVLNode* AVLTree::search(int skill_score) const {
    return buscar(raiz, skill_score);
}

AVLNode* AVLTree::getMenor(AVLNode* nodo) {
    while (nodo->izquierda != nullptr)
        nodo = nodo->izquierda;
    return nodo;
}

AVLNode* AVLTree::eliminar(AVLNode* nodo, int skill_score, int student_id) {
    if (nodo == nullptr) return nullptr;

    // Navegar usando el mismo criterio que insertar (menorQue)
    Student temp;
    temp.skill_score = skill_score;
    temp.student_id = student_id;

    if (menorQue(temp, nodo->data)) {
        nodo->izquierda = eliminar(nodo->izquierda, skill_score, student_id);
    } else if (menorQue(nodo->data, temp)) {
        nodo->derecha = eliminar(nodo->derecha, skill_score, student_id);
    } else {
        // Nodo encontrado
        if (nodo->izquierda == nullptr || nodo->derecha == nullptr) {
            AVLNode* hijo = nodo->izquierda ? nodo->izquierda : nodo->derecha;
            delete nodo;
            totalNodos--;
            return hijo;
        }
        // Dos hijos: reemplazar con sucesor inorden
        AVLNode* sucesor = getMenor(nodo->derecha);
        nodo->data = sucesor->data;
        nodo->derecha = eliminar(nodo->derecha, sucesor->data.skill_score, sucesor->data.student_id);
    }

    // Actualizar altura
    nodo->altura = 1 + maximo(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));

    // Rebalancear
    int balance = obtenerBalance(nodo);

    // LL
    if (balance > 1 && obtenerBalance(nodo->izquierda) >= 0)
        return rotarDerecha(nodo);
    // LR
    if (balance > 1 && obtenerBalance(nodo->izquierda) < 0) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    // RR
    if (balance < -1 && obtenerBalance(nodo->derecha) <= 0)
        return rotarIzquierda(nodo);
    // RL
    if (balance < -1 && obtenerBalance(nodo->derecha) > 0) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void AVLTree::remove(int skill_score, int student_id) {
    raiz = eliminar(raiz, skill_score, student_id);
}

// recorre de mayor a menor para el ranking
void AVLTree::mostrarRanking(AVLNode* nodo, int& pos, bool imprimir) const {
    if (nodo == nullptr) return;
    mostrarRanking(nodo->derecha, pos, imprimir);
    if (imprimir)
        cout << pos << ". " << nodo->data.full_name << " - Score: " << nodo->data.skill_score << endl;
    pos++;
    mostrarRanking(nodo->izquierda, pos, imprimir);
}

void AVLTree::printRanking(bool imprimir) const {
    if (raiz == nullptr) {
        if (imprimir)
            cout << "No hay estudiantes registrados." << endl;
        return;
    }
    int pos = 1;
    mostrarRanking(raiz, pos, imprimir);
}

int AVLTree::getHeight() const {
    return obtenerAltura(raiz);
}

int AVLTree::getRootBalanceFactor() const {
    return obtenerBalance(raiz);
}

int AVLTree::getNodeCount() const {
    return totalNodos;
}