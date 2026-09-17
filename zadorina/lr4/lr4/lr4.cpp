#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class TPerson
{
private:
    string fName, sName, tName;
    string ToUpper(string s)
    {
        for (char c : s) {
            c = toupper(c);
        }
        return s;
    }

    string ToLower(string s)
    {
        for (char c : s) {
            c = tolower(c);
        }
        return s;
    }

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

    string FullData()
    {
        return fName + " " + sName + " " + tName;
    }
    
    void WriteData(ofstream& file)
    {
        if (file.is_open())
            file << FullData();
    }
};

class TSpisok
{
private:
    vector<TPerson> persons;
public:
    TSpisok()
    {

    }
};

int main()
{
    string s = "fFFf";
    TPerson p(s, s, s);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
