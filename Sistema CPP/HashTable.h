#pragma once
#include "Student.h"
#include <iostream>
using namespace std;

// Tamaño primo para mejor distribución
const int HT_SIZE = 10007;

// Nodo de la lista enlazada para separate chaining
struct HTNode {
    Student data;
    HTNode* siguiente;

    HTNode(const Student& s) : data(s), siguiente(nullptr) {}
};

class HashTable {
public:
    HashTable();
    ~HashTable();

    // Inserta estudiante ignora si ya existe el student_id
    bool insertar(const Student& s);

    // Retorna puntero al estudiante o nullptr si no existe
    Student* buscar(int student_id);

    // Elimina por student_id retorna true si se elimino
    bool eliminar(int student_id);

    // Muestra colisiones, factor de carga, total elementos
    void mostrarEstadisticas() const;
    void printStats() const;

    int getTotalElementos() const;
    int getTotalColisiones() const;
    float getLoadFactor() const;

private:
    HTNode* tabla[HT_SIZE];
    int totalElementos;
    int totalColisiones;

    // Funcion hash student_id % HT_SIZE
    int hash(int student_id) const;
};

//  implementacion inline

inline HashTable::HashTable() : totalElementos(0), totalColisiones(0) {
    for (int i = 0; i < HT_SIZE; i++)
        tabla[i] = nullptr;
}

inline HashTable::~HashTable() {
    for (int i = 0; i < HT_SIZE; i++) {
        HTNode* actual = tabla[i];
        while (actual != nullptr) {
            HTNode* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
}

inline int HashTable::hash(int student_id) const {
    return student_id % HT_SIZE;
}

inline bool HashTable::insertar(const Student& s) {
    int idx = hash(s.student_id);

    // Verificar duplicado
    HTNode* actual = tabla[idx];
    while (actual != nullptr) {
        if (actual->data.student_id == s.student_id)
            return false; // ya existe
        actual = actual->siguiente;
    }

    // Contar colision si la cubeta ya tenía al menos un nodo
    if (tabla[idx] != nullptr)
        totalColisiones++;

    // Insertar al frente de la cadena
    HTNode* nuevo = new HTNode(s);
    nuevo->siguiente = tabla[idx];
    tabla[idx] = nuevo;
    totalElementos++;
    return true;
}

inline Student* HashTable::buscar(int student_id) {
    int idx = hash(student_id);
    HTNode* actual = tabla[idx];
    while (actual != nullptr) {
        if (actual->data.student_id == student_id)
            return &actual->data;
        actual = actual->siguiente;
    }
    return nullptr;
}

inline bool HashTable::eliminar(int student_id) {
    int idx = hash(student_id);
    HTNode* actual = tabla[idx];
    HTNode* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->data.student_id == student_id) {
            if (anterior == nullptr)
                tabla[idx] = actual->siguiente;
            else
                anterior->siguiente = actual->siguiente;
            delete actual;
            totalElementos--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

inline int HashTable::getTotalElementos() const {
    return totalElementos;
}

inline int HashTable::getTotalColisiones() const {
    return totalColisiones;
}

inline float HashTable::getLoadFactor() const {
    return (float)totalElementos / HT_SIZE;
}

inline void HashTable::mostrarEstadisticas() const {
    cout << "=== HashTable Stats ===" << endl;
    cout << "Total estudiantes : " << totalElementos << endl;
    cout << "Colisiones        : " << totalColisiones << endl;
    cout << "Factor de carga   : " << getLoadFactor() << endl;
}

inline void HashTable::printStats() const {
    mostrarEstadisticas();
}