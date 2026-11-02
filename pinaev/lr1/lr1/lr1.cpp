/*Энтропия файла*/

#include <iostream>
#include <fstream>
#include <windows.h>
#include <map>
#include <cmath>
using namespace std;

int entrophy(string fileName, double &ent, int &sizeFile, int &unique)
{
    ent = sizeFile = unique = 0;
    map<unsigned char, size_t> m;
    char cur;
    int i = 0;
    ifstream file(fileName, ios::binary);
    if (file.is_open())
    {
        while (file.get(cur))
        {
            m[static_cast<unsigned char>(cur)]++;
            i++;
        }
        file.close();
    }
    else
    {
        cout << "Не удалось открыть файл" << endl;
        return 0;
    }
    for (const auto& el : m)
        ent -= el.second / static_cast<double>(i) * log2(el.second / static_cast<double>(i));
    sizeFile = i;
    unique = m.size();
    return 1;
}


int main()
{
    SetConsoleCP(1251);
    setlocale(LC_ALL, "ru");

    while (true)
    {
        cout << "/////////////////////////////////////////////////////////"<<endl;
        cout << "Выберите действие\n1 - посчитать энтропию файла\n2 - создать файл\n";
        int com;
        string fileName;
        cin >> com;
        int sizeFile, unique;
        double ent;
        if (com == 1)
        {
            cout << "Введите название файла ";
            cin >> fileName;
            int ans = entrophy(fileName, ent, sizeFile, unique);
            if (ans == 1)
            {
                cout << "Энтропия файла = " << ent << endl;
                cout << "Длина файла = " << sizeFile << endl;
                cout << "Кол-во уникальных символов = " << unique << endl;
            }
        }
        else if (com == 2)
        {
            cout << "Введите название файла ";
            cin >> fileName;
            ofstream outFile(fileName, ios::binary);
            if (outFile.is_open())
            {
                cout << "Введите энтропию (от 0 до 7.8) ";
                double en;
                cin >> en;
                if (en < 0 || en>7.8)
                {
                    cout << "не допустимое значение энтропии";
                    continue;
                }
                cout << "Генерируем файл" << endl;
                double count = pow(2, en);
                int i = 0;
                for (; i < count; i++)
                {
                    outFile.put(static_cast<char>(i+33));
                }
                outFile.close();
                i += 32;
                cout << "Почти закончили"<<endl;
                while (true)
                {
                    entrophy(fileName, ent, sizeFile, unique);
                    if (abs(ent - en) <= 0.001)
                        break;
                    if (ent - en > 0.001)
                    {
                        outFile.open(fileName, ios::app);
                        outFile.put(static_cast<char>(i));
                        outFile.close();
                    }
                    else if (ent - en < -0.001)
                    {
                        outFile.open(fileName, ios::app);
                        outFile.put(static_cast<char>(++i));
                        outFile.close();
                    }
                    if (i > 255)
                    {
                        cout << "Ошибка слишком большой файл";
                        break;
                    }
                }
                
                entrophy(fileName, ent, sizeFile, unique);

                cout << "Энтропия файла = " << ent << endl;
                cout << "Длина файла = " << sizeFile << endl;
                cout << "Кол-во уникальных сиволов = " << unique << endl;
            }
            else
                cout << "Не удалось открыть файл";
        }
        else
            cout << "не распознанная команда\n";
    }

    return 0;
}