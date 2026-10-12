#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TPerson.h"
#include "Worker.h"
#include "Student.h"
#include "TSpisok.h"
using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    TSpisok spisok(filename);
    spisok.LoadFromFile();
    cout << "Загруженные данные:" << endl;
    spisok.PrintAll();

    bool isExit = false;
    while (!isExit)
    {
        int com;
        cout << "\nВыберите команду:" << endl;
        cout << "1 - добавить человека" << endl;
        cout << "2 - добавить студента" << endl;
        cout << "3 - добавить работника" << endl;
        cout << "4 - вывести список" << endl;
        cout << "5 - сохранить в файл" << endl;
        cout << "0 - выход" << endl;
        cin >> com;

        if (com == 0) { isExit = true; continue; }

        if (com == 4) { spisok.PrintAll(); continue; }
        if (com == 5) { spisok.SaveToFile(); cout << "Сохранено." << endl; continue; }

        string f, s, t;
        cout << "Введите фамилию: "; cin >> f;
        cout << "Введите имя: "; cin >> s;
        cout << "Введите отчество: "; cin >> t;

        switch (com)
        {
        case 1:
        {
            spisok.AddPerson(make_unique<TPerson>(f, s, t));
            cout << "Человек добавлен." << endl;
            break;
        }
        case 2:
        {
            int course;
            string group;
            cout << "Введите курс: "; cin >> course;
            cout << "Введите группу: "; cin >> group;
            spisok.AddPerson(make_unique<Student>(f, s, t, course, group));
            cout << "Студент добавлен." << endl;
            break;
        }
        case 3:
        {
            int tabel;
            cout << "Введите табельный номер: "; cin >> tabel;
            spisok.AddPerson(make_unique<Worker>(f, s, t, tabel));
            cout << "Работник добавлен." << endl;
            break;
        }
        default:
            cout << "Неизвестная команда" << endl;
            break;
        }
    }

    // Сохраняем всё при выходе
    spisok.SaveToFile();

}
