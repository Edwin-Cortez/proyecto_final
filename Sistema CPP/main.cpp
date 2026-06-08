#include "Student.h"
#include "AVL.h"
#include "HashTable.h"
#include "Graph.h"
#include "LinkedList.h"
#include "Queue.h"
#include "CSVLoader.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

// Mapeo de student_id a lista de proyectos (sin STL map)
struct StudentProjects {
    int student_id;
    ProjectList* lista;
};

StudentProjects proyectos[11500];
int totalProyectos = 0;

// Instancias globales de cada estructura
HashTable tablaHash;
AVLTree   avl;
Graph     graph;
Queue     cola;

// Tiempo de carga del dataset en milisegundos
double tiempoCargaDataset = 0.0;

// helpers

// Busca la lista de proyectos de un estudiante; nullptr si no existe
ProjectList* findProjectList(int student_id) {
    for (int i = 0; i < totalProyectos; i++) {
        if (proyectos[i].student_id == student_id)
            return proyectos[i].lista;
    }
    return nullptr;
}

// Agrega entrada nueva al arreglo de proyectos
void addProjectList(int student_id) {
    if (totalProyectos >= 11500) return;
    proyectos[totalProyectos].student_id = student_id;
    proyectos[totalProyectos].lista = new ProjectList();
    totalProyectos++;
}

// Elimina entrada y libera la lista de proyectos; compacta con el ultimo
void removeProjectList(int student_id) {
    for (int i = 0; i < totalProyectos; i++) {
        if (proyectos[i].student_id == student_id) {
            delete proyectos[i].lista;
            proyectos[i] = proyectos[totalProyectos - 1];
            totalProyectos--;
            return;
        }
    }
}

// Verifica si un estudiante existe en el hash
bool estaRegistrado(int student_id) {
    return tablaHash.buscar(student_id) != nullptr;
}

// menu

void mostrarMenu() {
    cout << "\n========================================" << endl;
    cout << "   RED SOCIAL ACADEMICA - MENU PRINCIPAL" << endl;
    cout << "   Edwin Oswaldo Cortez Tista Carnet: 9941-24-7386" << endl;
    cout << "========================================" << endl;
    cout << " 1.  Cargar estudiantes desde CSV"                     << endl;
    cout << " 2.  Registrar estudiante manualmente"                 << endl;
    cout << " 3.  Buscar estudiante por ID"                         << endl;
    cout << " 4.  Eliminar estudiante"                              << endl;
    cout << " 5.  Agregar proyecto a estudiante"                    << endl;
    cout << " 6.  Mostrar proyectos de estudiante"                  << endl;
    cout << " 7.  Conectar estudiantes"                             << endl;
    cout << " 8.  Verificar si dos estudiantes estan conectados"    << endl;
    cout << " 9.  Mostrar conexiones de estudiante"                 << endl;
    cout << "10.  Recorrer red con BFS"                             << endl;
    cout << "11.  Recorrer red con DFS"                             << endl;
    cout << "12.  Mostrar ranking academico"                        << endl;
    cout << "13.  Mostrar estadisticas de estructuras"              << endl;
    cout << "14.  Generar benchmark C++"                            << endl;
    cout << "15.  Salir"                                            << endl;
    cout << "========================================" << endl;
    cout << "Opcion: ";
}

int main() {
    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore();

        switch (opcion) {

            // Cargar desde CSV
            case 1: {
                cout << "Ruta del archivo [../dataset/estudiantes.csv]: ";
                string ruta;
                getline(cin, ruta);
                if (ruta.empty()) ruta = "../dataset/estudiantes.csv";

                auto t0 = chrono::high_resolution_clock::now();
                vector<Student> lista = CSVLoader::loadFromCSV(ruta);

                int cargados = 0, duplicados = 0;
                for (const Student& s : lista) {
                    if (estaRegistrado(s.student_id)) {
                        duplicados++;
                        continue;
                    }
                    tablaHash.insertar(s);
                    avl.insert(s);
                    graph.addStudent(s.student_id);
                    addProjectList(s.student_id);
                    cargados++;
                }

                auto t1 = chrono::high_resolution_clock::now();
                double ms = chrono::duration<double, milli>(t1 - t0).count();
                tiempoCargaDataset = ms; // guardar para estadisticas

                cout << "\nCarga completada:" << endl;
                cout << "  Cargados   : " << cargados   << endl;
                cout << "  Duplicados : " << duplicados  << endl;
                cout << "  Tiempo     : " << fixed << setprecision(2) << ms << " ms" << endl;
                break;
            }

            //  Registrar manualmente
            case 2: {
                int id, sem, score;
                float gpa;
                string nombre, carrera;

                cout << "ID del estudiante: ";
                cin >> id;
                cin.ignore();

                if (estaRegistrado(id)) {
                    cout << "Error: el ID " << id << " ya esta registrado." << endl;
                    break;
                }

                cout << "Nombre completo: ";
                getline(cin, nombre);
                cout << "Carrera: ";
                getline(cin, carrera);
                cout << "Semestre: ";
                cin >> sem;
                cout << "GPA: ";
                cin >> gpa;
                cout << "Skill score: ";
                cin >> score;
                cin.ignore();

                Student s(id, nombre, carrera, sem, gpa, score);
                tablaHash.insertar(s);
                avl.insert(s);
                graph.addStudent(id);
                addProjectList(id);

                cout << "Estudiante registrado correctamente." << endl;
                break;
            }

            //Buscar por ID
            case 3: {
                int id;
                cout << "ID del estudiante: ";
                cin >> id;
                cin.ignore();

                Student* s = tablaHash.buscar(id);
                if (s == nullptr)
                    cout << "Error: estudiante " << id << " no encontrado." << endl;
                else
                    s->print();
                break;
            }

            // Eliminar estudiante
            case 4: {
                int id;
                cout << "ID del estudiante a eliminar: ";
                cin >> id;
                cin.ignore();

                Student* s = tablaHash.buscar(id);
                if (s == nullptr) {
                    cout << "Error: estudiante " << id << " no encontrado." << endl;
                    break;
                }

                cout << "Eliminando: ";
                s->print();

                // Guardar skill_score antes de eliminar del hash
                int skillScore = s->skill_score;

                tablaHash.eliminar(id);
                avl.remove(skillScore, id);
                graph.removeStudent(id);
                removeProjectList(id);

                cout << "Estudiante " << id << " eliminado del sistema." << endl;
                break;
            }

            // Agregar proyecto
            case 5: {
                int id;
                cout << "ID del estudiante: ";
                cin >> id;
                cin.ignore();

                if (!estaRegistrado(id)) {
                    cout << "Error: estudiante " << id << " no registrado." << endl;
                    break;
                }

                int proj_id, año;
                string titulo, desc;

                cout << "ID del proyecto: ";
                cin >> proj_id;
                cin.ignore();
                cout << "Titulo: ";
                getline(cin, titulo);
                cout << "Descripcion: ";
                getline(cin, desc);
                cout << "Año de realizacion: ";
                cin >> año;
                cin.ignore();

                Project p(proj_id, titulo, desc, año);
                ProjectList* pl = findProjectList(id);
                if (!pl->addProject(p))
                    cout << "Error: proyecto " << proj_id << " ya existe para este estudiante." << endl;
                else
                    cout << "Proyecto agregado correctamente." << endl;
                break;
            }

            //  Mostrar proyectos
            case 6: {
                int id;
                cout << "ID del estudiante: ";
                cin >> id;
                cin.ignore();

                if (!estaRegistrado(id)) {
                    cout << "Error: estudiante " << id << " no registrado." << endl;
                    break;
                }

                ProjectList* pl = findProjectList(id);
                if (pl == nullptr) {
                    cout << "Error interno: lista de proyectos no encontrada." << endl;
                    break;
                }
                cout << "Proyectos del estudiante " << id << ":" << endl;
                pl->showProjects();
                break;
            }

            // Conectar estudiantes o gestionar solicitudes via cola circular
            case 7: {
                cout << "\n--- CONEXION DE ESTUDIANTES ---" << endl;
                cout << "1. Enviar solicitud de conexion" << endl;
                cout << "2. Procesar solicitudes pendientes" << endl;
                cout << "3. Ver solicitudes pendientes" << endl;
                cout << "Opcion: ";
                int sub;
                cin >> sub;
                cin.ignore();

                switch (sub) {
                    case 1: {
                        int id1, id2;
                        cout << "ID del primer estudiante: ";
                        cin >> id1;
                        cin.ignore();
                        cout << "ID del segundo estudiante: ";
                        cin >> id2;
                        cin.ignore();

                        if (!estaRegistrado(id1)) {
                            cout << "Error: estudiante " << id1 << " no registrado." << endl;
                            break;
                        }
                        if (!estaRegistrado(id2)) {
                            cout << "Error: estudiante " << id2 << " no registrado." << endl;
                            break;
                        }
                        if (id1 == id2) {
                            cout << "Error: no se puede conectar un estudiante consigo mismo." << endl;
                            break;
                        }
                        cola.enqueue(ConnectionRequest(id1, id2));
                        cout << "Solicitud encolada." << endl;
                        break;
                    }
                    case 2: {
                        if (cola.isEmpty()) {
                            cout << "No hay solicitudes pendientes." << endl;
                            break;
                        }
                        while (!cola.isEmpty()) {
                            ConnectionRequest r = cola.dequeue();
                            if (graph.connect(r.fromId, r.toId))
                                cout << "Conectados " << r.fromId << " y " << r.toId << endl;
                            else
                                cout << "No se pudo conectar " << r.fromId << " y " << r.toId
                                     << " (ya conectados o invalido)" << endl;
                        }
                        cout << "Solicitudes procesadas." << endl;
                        break;
                    }
                    case 3: {
                        cola.showPending();
                        break;
                    }
                    default: {
                        cout << "Sub-opcion invalida." << endl;
                        break;
                    }
                }
                break;
            }

            // Verificar conexion
            case 8: {
                int id1, id2;
                cout << "ID del primer estudiante: ";
                cin >> id1;
                cin.ignore();
                cout << "ID del segundo estudiante: ";
                cin >> id2;
                cin.ignore();

                if (graph.areConnected(id1, id2))
                    cout << "Los estudiantes " << id1 << " y " << id2 << " SI estan conectados." << endl;
                else
                    cout << "Los estudiantes " << id1 << " y " << id2 << " NO estan conectados." << endl;
                break;
            }

            // Mostrar conexiones
            case 9: {
                int id;
                cout << "ID del estudiante: ";
                cin >> id;
                cin.ignore();
                graph.showConnections(id);
                break;
            }

            //  BFS
            case 10: {
                int id;
                cout << "ID del estudiante inicial: ";
                cin >> id;
                cin.ignore();

                if (!estaRegistrado(id)) {
                    cout << "Error: estudiante " << id << " no registrado." << endl;
                    break;
                }
                graph.bfs(id);
                break;
            }

            //  DFS
            case 11: {
                int id;
                cout << "ID del estudiante inicial: ";
                cin >> id;
                cin.ignore();

                if (!estaRegistrado(id)) {
                    cout << "Error: estudiante " << id << " no registrado." << endl;
                    break;
                }
                graph.dfs(id);
                break;
            }

            // Ranking
            case 12: {
                cout << "\n=== RANKING ACADEMICO ===" << endl;
                avl.printRanking();
                break;
            }

            //  Estadisticas
            case 13: {
                // Sumar proyectos de todos los estudiantes
                int totalProy = 0;
                for (int i = 0; i < totalProyectos; i++) {
                    if (proyectos[i].lista != nullptr)
                        totalProy += proyectos[i].lista->getTotalProjects();
                }

                cout << "\n=== ESTADISTICAS DEL SISTEMA ===" << endl;
                cout << "Total estudiantes cargados : " << tablaHash.getTotalElementos()  << endl;
                cout << "Total conexiones creadas   : " << graph.getTotalEdges()          << endl;
                cout << "Total proyectos registrados: " << totalProy                      << endl;
                cout << "Total colisiones hash      : " << tablaHash.getTotalColisiones() << endl;
                cout << "Factor de carga hash       : " << fixed << setprecision(4)
                                                         << tablaHash.getLoadFactor()     << endl;
                cout << "Altura actual del AVL      : " << avl.getHeight()               << endl;
                cout << "Tiempo total de carga      : " << fixed << setprecision(2)
                                                         << tiempoCargaDataset << " ms"  << endl;
                break;
            }

            // Benchmark C++
            case 14: {
                cout << "Ejecutando benchmark con 10000 estudiantes sinteticos..." << endl;

                const int N       = 10000;
                const int ID_BASE = 20001;
                double tiempos[6] = {};

                // Instancias locales para no contaminar el sistema global
                HashTable* bhash  = new HashTable();
                AVLTree*   bavl   = new AVLTree();
                Graph*     bgraph = new Graph();

                // a) Insercion en HashTable
                {
                    auto t0 = chrono::high_resolution_clock::now();
                    for (int i = 0; i < N; i++) {
                        int id = ID_BASE + i;
                        Student s(id, "Bench", "CS", 1, 3.0f, id % 500);
                        bhash->insertar(s);
                    }
                    tiempos[0] = chrono::duration<double, milli>(
                        chrono::high_resolution_clock::now() - t0).count();
                }

                // b) Busqueda en HashTable
                {
                    auto t0 = chrono::high_resolution_clock::now();
                    for (int i = 0; i < N; i++)
                        bhash->buscar(ID_BASE + i);
                    tiempos[1] = chrono::duration<double, milli>(
                        chrono::high_resolution_clock::now() - t0).count();
                }

                // c) Insercion en AVL
                {
                    auto t0 = chrono::high_resolution_clock::now();
                    for (int i = 0; i < N; i++) {
                        int id = ID_BASE + i;
                        Student s(id, "Bench", "CS", 1, 3.0f, id % 500);
                        bavl->insert(s);
                    }
                    tiempos[2] = chrono::duration<double, milli>(
                        chrono::high_resolution_clock::now() - t0).count();
                }

                // d) Recorrido inorden del AVL sin imprimir
                {
                    auto t0 = chrono::high_resolution_clock::now();
                    bavl->printRanking(false);
                    tiempos[3] = chrono::duration<double, milli>(
                        chrono::high_resolution_clock::now() - t0).count();
                }

                // e) Construir grafo con pares consecutivos y medir BFS
                {
                    for (int i = 0; i < N; i++)
                        bgraph->addStudent(ID_BASE + i);
                    for (int i = 0; i < N - 1; i++)
                        bgraph->connect(ID_BASE + i, ID_BASE + i + 1);

                    auto t0 = chrono::high_resolution_clock::now();
                    bgraph->bfs(ID_BASE, false);
                    tiempos[4] = chrono::duration<double, milli>(
                        chrono::high_resolution_clock::now() - t0).count();
                }

                // f) DFS desde el primer vertice
                {
                    auto t0 = chrono::high_resolution_clock::now();
                    bgraph->dfs(ID_BASE, false);
                    tiempos[5] = chrono::duration<double, milli>(
                        chrono::high_resolution_clock::now() - t0).count();
                }

                delete bhash;
                delete bavl;
                delete bgraph;

                // Escribir resultados a CSV
                string rutaCSV = "../dataset/cpp_results.csv";
                ofstream archivo(rutaCSV);
                if (!archivo.is_open()) {
                    cout << "Error: no se pudo crear " << rutaCSV << endl;
                    break;
                }

                archivo << fixed << setprecision(2);
                archivo << "language,operation,structure,records,time_ms\n";
                archivo << "C++,insert,HashTable,"  << N << "," << tiempos[0] << "\n";
                archivo << "C++,search,HashTable,"  << N << "," << tiempos[1] << "\n";
                archivo << "C++,insert,AVL,"        << N << "," << tiempos[2] << "\n";
                archivo << "C++,traversal,AVL,"     << N << "," << tiempos[3] << "\n";
                archivo << "C++,bfs,Graph,"          << N << "," << tiempos[4] << "\n";
                archivo << "C++,dfs,Graph,"          << N << "," << tiempos[5] << "\n";
                archivo.close();

                cout << "\n=== RESULTADOS DEL BENCHMARK ===" << endl;
                cout << fixed << setprecision(2);
                cout << "  HashTable insert  : " << tiempos[0] << " ms" << endl;
                cout << "  HashTable search  : " << tiempos[1] << " ms" << endl;
                cout << "  AVL insert        : " << tiempos[2] << " ms" << endl;
                cout << "  AVL traversal     : " << tiempos[3] << " ms" << endl;
                cout << "  Graph BFS         : " << tiempos[4] << " ms" << endl;
                cout << "  Graph DFS         : " << tiempos[5] << " ms" << endl;
                cout << "\nResultados escritos en: " << rutaCSV << endl;
                break;
            }

            //  Salir
            case 15: {
                cout << "\nHasta luego. Cerrando el sistema..." << endl;
                break;
            }

            default: {
                cout << "Opcion invalida. Ingrese un numero entre 1 y 15." << endl;
                break;
            }
        }

    } while (opcion != 15);

    // Liberar memoria de todas las listas de proyectos
    for (int i = 0; i < totalProyectos; i++)
        delete proyectos[i].lista;

    return 0;
}