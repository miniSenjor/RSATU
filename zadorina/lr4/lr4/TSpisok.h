#pragma once
#include "TPerson.h"
#include "Worker.h"
#include "Student.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
class TSpisok
{
private:
    vector<unique_ptr<TPerson>> persons;
    string filename;
public:
    TSpisok(string f);
    ~TSpisok();
    void LoadFromFile();

    // Сохранение всего списка в файл
    void SaveToFile();

    // Добавление нового человека (в память)
    void AddPerson(unique_ptr<TPerson> p);

    // Запись одного человека в конец файла
    void AppendToFile(TPerson* p);

    // Для отладки: вывод всех
    void PrintAll() const;
};