#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class TPerson
{
private:
    string ToUpper(string s)
    {
        for (char& c : s) {
            c = toupper(c);
        }
        return s;
    }

    string ToLower(string s)
    {
        for (char& c : s) {
            c = tolower(c);
        }
        return s;
    }
protected:
    string fName, sName, tName;

public:
    TPerson(ifstream& file)
    {
        string f, s, t;
        file >> f >> s >> t;
        SetFName(f);
        SetSName(s);
        SetTName(t);
    }
    TPerson(string f, string s, string t)
    {
        SetFName(f);
        SetSName(s);
        SetTName(t);
    }

    void SetFName(string f)
    {
        fName = ToUpper(f);
    }

    void SetSName(string s)
    {
        sName = ToLower(s);
    }

    void SetTName(string t)
    {
        tName = ToLower(t);
    }

    virtual string FullData()
    {
        return fName + " " + sName + " " + tName;
    }

    virtual string GetType() const { return "P"; }

    void WriteData(ofstream& file)
    {
        if (file.is_open())
            file << FullData();
    }
};

class Worker : public TPerson
{
protected:
    int tabelNumber;
public:
    Worker(string f, string s, string t, int n) : TPerson(f, s, t)
    {
        SetTNumber(n);
    }
    void SetTNumber(int number)
    {
        tabelNumber = number;
    }
    string FullData() override
    {
        return TPerson::FullData() + " " + to_string(tabelNumber);
    }
    string GetType() const override { return "W"; }
};

class Student : public TPerson
{
protected:
    int course;
    string group;
public:
    Student(string f, string s, string t, int course, string group) : TPerson(f, s, t)
    {
        SetGroup(group);
        SetCourse(course);
    }
    void SetGroup(string g)
    {
        group = g;
    }
    void SetCourse(int c)
    {
        course = c;
    }
    string FullData() override
    {
        return TPerson::FullData() + " " + to_string(course) + " " + group;
    }
    string GetType() const override { return "S"; }
};

class TSpisok
{
private:
    vector<unique_ptr<TPerson>> persons;
public:
    void LoadFromFile(const string& filename)
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
    void SaveToFile(const string& filename)
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
    void AddPerson(unique_ptr<TPerson> p)
    {
        persons.push_back(move(p));
    }

    // Запись одного человека в конец файла
    void AppendToFile(const string& filename, TPerson* p)
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
    void PrintAll() const
    {
        for (const auto& p : persons) {
            cout << p->GetType() << " " << p->FullData() << endl;
        }
    }
};

int main()
{
    setlocale(LC_ALL, "ru");
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    TSpisok spisok;
    spisok.LoadFromFile(filename);
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
        if (com == 5) { spisok.SaveToFile(filename); cout << "Сохранено." << endl; continue; }

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
    spisok.SaveToFile(filename);


}
