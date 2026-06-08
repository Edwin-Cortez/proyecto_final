cmake_minimum_required(VERSION 4.1)
project(proyectofinal)

set(CMAKE_CXX_STANDARD 20)

add_executable(proyectofinal main.cpp
        "Sistema CPP/AVL.cpp"
        "Sistema CPP/AVL.h"
        "Sistema CPP/CSVLoader.h"
        "Sistema CPP/Graph.h"
        "Sistema CPP/main.cpp"
        "Sistema CPP/HashTable.h"
        "Sistema CPP/LinkedList.h"
        "Sistema CPP/Queue.h"
        "Sistema CPP/Estudiante.h"
)
