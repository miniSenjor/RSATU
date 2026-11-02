//Дескриптор
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

vector<int> AddressToIndexes(int addr, int a1, int n, vector<int>& d,
    vector<pair<int, int>>& pairs)
{
    vector<int> indexes(n);
    int offset = addr - a1;

    for (int i = 0; i < n - 1; ++i) {
        indexes[i] = pairs[i].first + offset / d[i];
        offset %= d[i];
    }
    indexes[n - 1] = pairs[n - 1].first + offset / d[n - 1];

    for (int i = 0; i < n; ++i) {
        if (indexes[i] < pairs[i].first || indexes[i] > pairs[i].second) {
            return {};
        }
    }
    return indexes;
}

vector<int> InputIndex(int n, vector<pair<int, int>> pairs)
{
    cout << "Введите индексы: ";
    vector<int> indexes(n);
    for (int i = 0; i < n; i++)
    {
        while (true)
        {
            cin >> indexes[i];
            if (indexes[i] >= pairs[i].first && indexes[i] <= pairs[i].second)
                break;
        }
    }
    return indexes;
}

int IndexesToAddress(vector<int> indexes, int a0, int n, vector<int> d)
{
    int address = a0;
    for (int i = 0; i < n; i++)
    {
        address += indexes[i] * d[i];
    }
    return address;
}

int main()
{
    SetConsoleCP(1251);
    setlocale(LC_ALL, "ru");

    int l = 0, a1, n, a0, type;
    cout << "Введите размерность массива: ";
    cin >> n;
    vector<int> d(n);
    vector<pair<int, int>> pairs(n);
    while (l <= 0)
    {
        cout << "Введите кол-во байт для элемента: ";
        cin >> l;
    }
    d[n - 1] = l;
    cout << "Введите начало a1: ";
    cin >> a1;
    a0 = a1;
    for (int i = 0; i < n; i++) {
        while (true) {
            cout << "Введите " << i + 1 << " граничную пару:";
            cin >> pairs[i].first >> pairs[i].second;
            if (pairs[i].first <= pairs[i].second)
                break;
            cout << "Неверно заданы границы" << endl;
        }
    }
    long lenMas = 1;
    for (int i = 0; i < n; i++)
    {
        lenMas *= pairs[i].second - pairs[i].first + 1;
    }
    lenMas += a1 + 1000;
    vector<char> mass(10000000);
    a0 -= d[n - 1] * pairs[n - 1].first;
    for (int i = n - 2; i >= 0; i--) {
        d[i] = d[i + 1] * (pairs[i + 1].second - pairs[i + 1].first + 1);
        a0 -= d[i] * pairs[i].first;
    }
    cout << "Дескриптор:" << endl;
    cout << "n = " << n << "; l = " << l << "; a0 = " << a0 << "; a1 = " << a1 << ";" << endl;
    for (int i = 0; i < n; i++) {
        cout << "D" << i + 1 << " = " << d[i] << endl;
    }
    cout << "Адр([j1";
    for (int i = 2; i <= n; i++) {
        cout << ", j" << i;
    }
    cout << "]) = " << a0;
    for (int i = 0; i < n; i++) {
        cout << "+" << d[i] << "*j" << i + 1;
    }
    cout << endl;
    do {
        cout << "1 - записать, 2 - прочитать, 3 - записать по адресу, 0 - выход" << endl;
        cin >> type;
        switch (type)
        {
            case 1:
            {
                vector<int> indexs = InputIndex(n, pairs);
                cout << "Введите строку: ";
                string s = "";
                cin.ignore();
                while (true)
                {
                    getline(cin, s);
                    if (s.length() != l)
                        cout << "Введеная строка должна быть длинны " << l << endl;
                    else
                        break;
                }
                int address = IndexesToAddress(indexs, a0, n, d);
                for (int i = 0; i < l; i++)
                    mass[address + i] = s[i];
                break;
            }
            
            case 2:
            {
                vector<int> indexs = InputIndex(n, pairs);
                int adress = IndexesToAddress(indexs, a0, n, d);
                cout << "Адр(";
                for (int i = 0; i < n; i++)
                {
                    cout << indexs[i];
                    if (i < n - 1)
                        cout << ", ";
                }
                cout << ")=";
                for (int i = 0; i < l; i++)
                {
                    cout << mass[i+adress];
                }
                cout << endl;
                string com;
                while (true)
                {
                    cout << "Хотите изменить в одну из ячеек?"<<endl;
                    cout << "1 - да, 0 - нет"<<endl;
                    cin >> com;
                    if (com == "y" || com == "1")
                    {
                        cout << "Введите номер байта: ";
                        int num;
                        cin >> num;
                        cout << "Введите символ: ";
                        char c;
                        cin >> c;
                        mass[adress + num - 1] = c;
                    }
                    else if (com == "n" || com == "0")
                        break;
                    else
                        cout << "команда не распознана";
                }
                break;
            }
            case 3:
            {
                cout << "Введите адрес: ";
                int address;
                cin >> address;
                cout << "Введите символ: ";
                string s;
                cin >> s;
                mass[address] = s[0];
            break;
            }
            case 4:
            {
                int add;
                cin >> add;
                vector<int> v = AddressToIndexes(add, a1, n, d, pairs);
                for (int i = 0; i < n; i++)
                {
                    cout << v[i] << " ";
                }
                cout << endl;
            }
        }
    } while (type);
}

