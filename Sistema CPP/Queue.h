#pragma once
#include <iostream>
using namespace std;

// Solicitud de conexion entre dos estudiantes
struct ConnectionRequest {
    int fromId;
    int toId;
    ConnectionRequest() : fromId(0), toId(0) {}
    ConnectionRequest(int f, int t) : fromId(f), toId(t) {}
};

// Cola circular con arreglo fijo para solicitudes pendientes
class Queue {
public:
    Queue();
    ~Queue();

    // Encola solicitud; false si la cola esta llena
    bool enqueue(ConnectionRequest r);

    // Desencola y retorna la solicitud; retorna request vacío si esta vacia
    ConnectionRequest dequeue();

    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;

    // Muestra todas las solicitudes pendientes sin modificar la cola
    void showPending() const;

private:
    static const int CAPACITY = 500;

    ConnectionRequest* data;
    int front;  // indice del primer elemento
    int back;   // indice donde se insertara el proximo
    int size;   // cantidad de elementos actuales
};

// implementación inline
inline Queue::Queue() : front(0), back(0), size(0) {
    data = new ConnectionRequest[CAPACITY];
}

inline Queue::~Queue() {
    delete[] data;
}

inline bool Queue::isEmpty() const {
    return size == 0;
}

inline bool Queue::isFull() const {
    return size == CAPACITY;
}

inline int Queue::getSize() const {
    return size;
}

inline bool Queue::enqueue(ConnectionRequest r) {
    if (isFull()) return false;

    data[back] = r;
    back = (back + 1) % CAPACITY; // avance circular
    size++;
    return true;
}

inline ConnectionRequest Queue::dequeue() {
    if (isEmpty()) return ConnectionRequest(); // vacio retorna default

    ConnectionRequest r = data[front];
    front = (front + 1) % CAPACITY; // avance circular
    size--;
    return r;
}

inline void Queue::showPending() const {
    if (isEmpty()) {
        cout << "  (no hay solicitudes pendientes)" << endl;
        return;
    }
    cout << "Solicitudes pendientes (" << size << "):" << endl;
    // Recorre sin modificar front/back
    for (int i = 0; i < size; i++) {
        int idx = (front + i) % CAPACITY;
        cout << "  " << data[idx].fromId << " -> " << data[idx].toId << endl;
    }
}