#pragma once
#include <string>
#include <iostream>
using namespace std;

// Proyecto asociado a un estudiante
struct Project {
    int project_id;
    string title;
    string description;
    int completion_year;

    Project() : project_id(0), completion_year(0) {}
    Project(int id, string t, string desc, int year)
        : project_id(id), title(t), description(desc), completion_year(year) {}
};

// Nodo de la lista enlazada simple
struct ProjectNode {
    Project data;
    ProjectNode* siguiente;
    ProjectNode(Project p) : data(p), siguiente(nullptr) {}
};

class ProjectList {
public:
    ProjectList() : head(nullptr), totalProjects(0) {}
    ~ProjectList();

    // Inserta al frente false si project_id ya existe
    bool addProject(Project p);

    // Elimina por project_id false si no existe
    bool removeProject(int project_id);

    // Retorna puntero al proyecto o nullptr si no existe
    Project* searchProject(int project_id);

    // Muestra todos los proyectos de la lista
    void showProjects() const;

    int getTotalProjects() const;

private:
    ProjectNode* head;
    int totalProjects;
};

// implementacion inline

inline ProjectList::~ProjectList() {
    // Libera todos los nodos de la lista
    ProjectNode* actual = head;
    while (actual != nullptr) {
        ProjectNode* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

inline bool ProjectList::addProject(Project p) {
    // Verificar que no exista project_id duplicado
    ProjectNode* actual = head;
    while (actual != nullptr) {
        if (actual->data.project_id == p.project_id)
            return false;
        actual = actual->siguiente;
    }

    // Insertar al frente en O(1)
    ProjectNode* nuevo = new ProjectNode(p);
    nuevo->siguiente = head;
    head = nuevo;
    totalProjects++;
    return true;
}

inline bool ProjectList::removeProject(int project_id) {
    ProjectNode* actual = head;
    ProjectNode* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->data.project_id == project_id) {
            if (anterior == nullptr)
                head = actual->siguiente;
            else
                anterior->siguiente = actual->siguiente;
            delete actual;
            totalProjects--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false; // no encontrado
}

inline Project* ProjectList::searchProject(int project_id) {
    ProjectNode* actual = head;
    while (actual != nullptr) {
        if (actual->data.project_id == project_id)
            return &actual->data;
        actual = actual->siguiente;
    }
    return nullptr;
}

inline void ProjectList::showProjects() const {
    if (head == nullptr) {
        cout << "  (sin proyectos registrados)" << endl;
        return;
    }
    ProjectNode* actual = head;
    while (actual != nullptr) {
        cout << "  [" << actual->data.project_id << "] "
             << actual->data.title
             << " (" << actual->data.completion_year << ")"
             << " - " << actual->data.description << endl;
        actual = actual->siguiente;
    }
}

inline int ProjectList::getTotalProjects() const {
    return totalProjects;
}