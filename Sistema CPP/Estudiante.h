#pragma once
#include <string>
#include <iostream>
using namespace std;

struct Student {
    int student_id;
    string full_name;
    string career;
    int semester;
    float gpa;
    int skill_score;

    Student() : student_id(0), semester(0), gpa(0), skill_score(0) {}

    Student(int id, string name, string car, int sem, float g, int score)
        : student_id(id), full_name(name), career(car),
          semester(sem), gpa(g), skill_score(score) {}

    void print() const {
        cout << "[" << student_id << "] " << full_name
             << " | " << career
             << " | Sem: " << semester
             << " | GPA: " << gpa
             << " | Score: " << skill_score << endl;
    }
};