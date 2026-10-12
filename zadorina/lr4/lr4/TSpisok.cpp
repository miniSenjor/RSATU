#include "TSpisok.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "TPerson.h"

#include <vector>
using namespace std;

TSpisok::TSpisok(string f)
{
    filename = f;
}

TSpisok::~TSpisok()
{
    SaveToFile();
    persons.clear();
}


void TSpisok::LoadFromFile()
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл " << filename << endl;
        return;
    }
    persons.clear();
    char type;
    while (file >> type) {
        if (type == 'P') {
            string f, s, t;
            file >> f >> s >> t;
            persons.push_back(make_unique<TPerson>(f, s, t));
        }
        else if (type == 'W') {
            string f, s, t;
            int n;
            file >> f >> s >> t >> n;
            persons.push_back(make_unique<Worker>(f, s, t, n));
        }
        else if (type == 'S') {
            string f, s, t, group;
            int course;
            file >> f >> s >> t >> course >> group;
            persons.push_back(make_unique<Student>(f, s, t, course, group));
        }
        else {
            // пропускаем некорректную строку
            string dummy;
            getline(file, dummy);
        }
    }
    file.close();
}

// Сохранение всего списка в файл
void TSpisok::SaveToFile()
{
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для записи " << filename << endl;
        return;
    }
    for (const auto& p : persons) {
        file << p->GetType() << " " << p->FullData() << endl;
    }
    file.close();
}

// Добавление нового человека (в память)
void TSpisok::AddPerson(unique_ptr<TPerson> p)
{
    persons.push_back(move(p));
}

// Запись одного человека в конец файла
void TSpisok::AppendToFile(TPerson* p)
{
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для добавления " << filename << endl;
        return;
    }
    file << p->GetType() << " " << p->FullData() << endl;
    file.close();
}

// Для отладки: вывод всех
void TSpisok::PrintAll() const
{
    for (const auto& p : persons) {
        cout << p->GetType() << " " << p->FullData() << endl;
    }
}
