# proyecto\_final:



\# Red Social Académica - Proyecto Final

\## Programación III - Universidad Mariano Gálvez de Guatemala



\## Descripción

Sistema la cual simula una red social académica universitaria implementando 

estructuras de datos avanzadas tanto en C++ y en un benchmark comparativo en Java.



\## Estructuras implementadas en C++

\- \*\*HashTable\*\* — Separate chaining, clave: student\_id

\- \*\*AVL Tree\*\* — Ranking académico por skill\_score

\- \*\*Grafo no dirigido\*\* — Conexiones entre estudiantes, BFS y DFS

\- \*\*Lista enlazada\*\* — Proyectos académicos por estudiante

\- \*\*Cola circular\*\* — Solicitudes de conexión pendientes



\## Estructura del repositorio

```

proyecto-final/

├── cpp-system/

│   ├── main.cpp

│   ├── Student.h

│   ├── AVL.h

│   ├── AVL.cpp

│   ├── HashTable.h

│   ├── CSVLoader.h

│   ├── Graph.h

│   ├── LinkedList.h

│   └── Queue.h

├── java-benchmark/

│   └── Benchmark.java

├── dataset/

│   ├── estudiantes.csv

│   ├── cpp\_results.csv

│   └── java\_results.csv

├── evidencias/

└── README.md

```



\## Compilar y ejecutar C++

```

cd cpp-system

g++ -std=c++17 -O2 main.cpp AVL.cpp -o sistema

./sistema

```



\## Compilar y ejecutar Java

```

cd java-benchmark

javac Benchmark.java

java Benchmark

```



\## Dataset

\- 10,000 estudiantes precargados

\- IDs del 1001 al 11000

\- Archivo: dataset/estudiantes.csv



\## Benchmark comparativo

| Operación | C++ | Java |

|-----------|-----|------|

| HashTable insert 10k | 0.88 ms | 0.57 ms |

| HashTable search 10k | 0.03 ms | 0.46 ms |

| AVL insert 10k | 2.62 ms | 2.59 ms |

| AVL traversal 10k | 1.07 ms | 0.74 ms |



\## Autor

Edwin Oswaldo Cortez Tista - Universidad Mariano Gálvez de Guatemala

