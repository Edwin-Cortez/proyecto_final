#pragma once
#include "Student.h"
#include <iostream>
using namespace std;

// Nodo de la lista de adyacencia
struct AdjNode {
    int studentId;
    AdjNode* siguiente;
    AdjNode(int id) : studentId(id), siguiente(nullptr) {}
};

// Entrada de la lista de adyacencia para un vertice
struct AdjList {
    int studentId;
    AdjNode* head;
    AdjList() : studentId(-1), head(nullptr) {}
};

class Graph {
public:
    Graph();
    ~Graph();

    // Agrega un vertice si no existe
    void addStudent(int studentId);

    // Elimina vertice y todas las aristas que lo referencian
    void removeStudent(int studentId);

    // Arista no dirigida; false si ya existe o es el mismo id
    bool connect(int id1, int id2);

    // Verifica si dos estudiantes estan conectados
    bool areConnected(int id1, int id2);

    // Muestra los vecinos de un estudiante
    void showConnections(int studentId);

    // Recorrido BFS desde un nodo usa arreglo manual como cola
    void bfs(int startId, bool imprimir = true);

    // Recorrido DFS desde un nodo usa arreglo manual como pila
    void dfs(int startId, bool imprimir = true);

    int getTotalEdges() const;
    int getTotalVertices() const;

private:
    static const int MAX_CAP = 11500;

    AdjList* lists;     // arreglo dinamico de listas de adyacencia
    int capacity;
    int totalVertices;
    int totalEdges;

    // Busqueda lineal por studentId retorna -1 si no existe
    int findIndex(int studentId) const;

    // Agrega nuevo vertice al arreglo; retorna índice asignado
    int addVertex(int studentId);
};

// implementacion inline

inline Graph::Graph() : capacity(MAX_CAP), totalVertices(0), totalEdges(0) {
    lists = new AdjList[capacity];
}

inline Graph::~Graph() {
    // Libera todos los nodos de cada lista de adyacencia
    for (int i = 0; i < totalVertices; i++) {
        AdjNode* actual = lists[i].head;
        while (actual != nullptr) {
            AdjNode* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
    delete[] lists;
}

inline int Graph::findIndex(int studentId) const {
    for (int i = 0; i < totalVertices; i++) {
        if (lists[i].studentId == studentId)
            return i;
    }
    return -1;
}

inline int Graph::addVertex(int studentId) {
    if (totalVertices >= capacity)
        return -1; // capacidad máxima alcanzada
    int idx = totalVertices;
    lists[idx].studentId = studentId;
    lists[idx].head = nullptr;
    totalVertices++;
    return idx;
}

inline void Graph::addStudent(int studentId) {
    if (findIndex(studentId) == -1)
        addVertex(studentId);
}

inline void Graph::removeStudent(int studentId) {
    int idx = findIndex(studentId);
    if (idx == -1) return;

    // Liberar lista de adyacencia propia
    AdjNode* actual = lists[idx].head;
    while (actual != nullptr) {
        AdjNode* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }

    // Eliminar referencias a este estudiante en listas de otros vértices
    for (int i = 0; i < totalVertices; i++) {
        if (i == idx) continue;
        AdjNode* cur = lists[i].head;
        AdjNode* prev = nullptr;
        while (cur != nullptr) {
            if (cur->studentId == studentId) {
                if (prev == nullptr)
                    lists[i].head = cur->siguiente;
                else
                    prev->siguiente = cur->siguiente;
                delete cur;
                totalEdges--; // cada arista se cuenta una vez
                break;
            }
            prev = cur;
            cur = cur->siguiente;
        }
    }

    // Compactar el arreglo moviendo el último vértice al hueco
    lists[idx] = lists[totalVertices - 1];
    lists[totalVertices - 1].studentId = -1;
    lists[totalVertices - 1].head = nullptr;
    totalVertices--;
}

inline bool Graph::connect(int id1, int id2) {
    // No conectar consigo mismo
    if (id1 == id2) return false;

    // Agregar vertices si no existen
    if (findIndex(id1) == -1) addVertex(id1);
    if (findIndex(id2) == -1) addVertex(id2);

    int idx1 = findIndex(id1);
    int idx2 = findIndex(id2);

    // Verificar duplicado (basta revisar un lado, el grafo es no dirigido)
    AdjNode* cur = lists[idx1].head;
    while (cur != nullptr) {
        if (cur->studentId == id2) return false;
        cur = cur->siguiente;
    }

    // Insertar arista en ambas listas (al frente)
    AdjNode* nodo1 = new AdjNode(id2);
    nodo1->siguiente = lists[idx1].head;
    lists[idx1].head = nodo1;

    AdjNode* nodo2 = new AdjNode(id1);
    nodo2->siguiente = lists[idx2].head;
    lists[idx2].head = nodo2;

    totalEdges++;
    return true;
}

inline bool Graph::areConnected(int id1, int id2) {
    int idx = findIndex(id1);
    if (idx == -1) return false;
    AdjNode* cur = lists[idx].head;
    while (cur != nullptr) {
        if (cur->studentId == id2) return true;
        cur = cur->siguiente;
    }
    return false;
}

inline void Graph::showConnections(int studentId) {
    int idx = findIndex(studentId);
    if (idx == -1) {
        cout << "Estudiante " << studentId << " no encontrado en el grafo." << endl;
        return;
    }
    cout << "Conexiones de " << studentId << ": ";
    AdjNode* cur = lists[idx].head;
    if (cur == nullptr) {
        cout << "(sin conexiones)" << endl;
        return;
    }
    while (cur != nullptr) {
        cout << cur->studentId;
        if (cur->siguiente != nullptr) cout << " -> ";
        cur = cur->siguiente;
    }
    cout << endl;
}

inline void Graph::bfs(int startId, bool imprimir) {
    int idx = findIndex(startId);
    if (idx == -1) {
        if (imprimir)
            cout << "Estudiante " << startId << " no encontrado." << endl;
        return;
    }

    bool visited[MAX_CAP] = {};
    int queue[MAX_CAP];
    int front = 0, back = 0;

    // encolar vertice inicial
    queue[back++] = startId;
    visited[idx] = true;

    if (imprimir)
        cout << "BFS desde " << startId << ": ";
    while (front < back) {
        int current = queue[front++];
        if (imprimir)
            cout << current << " ";

        int curIdx = findIndex(current);
        AdjNode* vecino = lists[curIdx].head;
        while (vecino != nullptr) {
            int vecIdx = findIndex(vecino->studentId);
            if (vecIdx != -1 && !visited[vecIdx]) {
                visited[vecIdx] = true;
                queue[back++] = vecino->studentId;
            }
            vecino = vecino->siguiente;
        }
    }
    if (imprimir)
        cout << endl;
}

inline void Graph::dfs(int startId, bool imprimir) {
    int idx = findIndex(startId);
    if (idx == -1) {
        if (imprimir)
            cout << "Estudiante " << startId << " no encontrado." << endl;
        return;
    }

    bool visited[MAX_CAP] = {};
    int stack[MAX_CAP];
    int top = 0;

    // apilar vertice inicial
    stack[top++] = startId;

    if (imprimir)
        cout << "DFS desde " << startId << ": ";
    while (top > 0) {
        int current = stack[--top];
        int curIdx = findIndex(current);

        if (visited[curIdx]) continue;
        visited[curIdx] = true;
        if (imprimir)
            cout << current << " ";

        // apilar vecinos no visitados
        AdjNode* vecino = lists[curIdx].head;
        while (vecino != nullptr) {
            int vecIdx = findIndex(vecino->studentId);
            if (vecIdx != -1 && !visited[vecIdx])
                stack[top++] = vecino->studentId;
            vecino = vecino->siguiente;
        }
    }
    if (imprimir)
        cout << endl;
}

inline int Graph::getTotalEdges() const {
    return totalEdges;
}

inline int Graph::getTotalVertices() const {
    return totalVertices;
}