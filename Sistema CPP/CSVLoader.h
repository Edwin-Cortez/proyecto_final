#pragma once
#include "Student.h"
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class CSVLoader {
public:
    static vector<Student> loadFromCSV(string path) {
        vector<Student> students;
        ifstream file(path);

        if (!file.is_open()) {
            cout << "Error: no se pudo abrir el archivo " << path << endl;
            return students;
        }

        string line;
        getline(file, line); // saltar encabezado

        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string token;
            Student s;

            getline(ss, token, ','); s.student_id = stoi(token);
            getline(ss, s.full_name, ',');
            getline(ss, s.career, ',');
            getline(ss, token, ','); s.semester = stoi(token);
            getline(ss, token, ','); s.gpa = stof(token);
            getline(ss, token, ','); s.skill_score = stoi(token);

            students.push_back(s);
        }

        file.close();
        return students;
    }
};