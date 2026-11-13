#include <iostream>
#include <vector>
#include <windows.h>
#include "Matrix.h"

using namespace std;

void InputMatrix(int n, int m, Matrix *&a, Matrix *&b)
{
    cout << "Введите матрицу A" << endl;
    a = new Matrix(n, m);
    a->ReadMas();
    cout << "Введите матрицу B" << endl;
    b = new Matrix(n, m);
    b->ReadMas();
}

void InputNM(int& n, int& m, Matrix *&a, Matrix *&b)
{
    cout << "Введите размеры массива" << endl;
    cout << "n = ";
    cin >> n;
    cout << "m = ";
    cin >> m;
    InputMatrix(n, m, a, b);
}

int main()
{
    SetConsoleOutputCP(1251);
    int n = 0, m = 0;
    Matrix *a = new Matrix();
    Matrix *b = new Matrix();
    bool hasInputNM = false;
    while (true)
    {
        cout << "Выберите команду" << endl;
        cout << "1 ввод размера матрицы " << endl;
        cout << "2 ввод матрицы " << endl;
        cout << "3 A + B " << endl;
        cout << "4 A + число " << endl;
        cout << "5 A * B " << endl;
        cout << "6 A * число " << endl;
        cout << "7 A++ " << endl;
        cout << "0 выход " << endl;
        int com;
        cin >> com;
        try
        {
            switch (com)
            {
            case 1:
                cout << "Ввод размера матрицы" << endl;
                InputNM(n, m, a, b);
                hasInputNM = true;
                break;
            case 2:
                if (!hasInputNM)
                {
                    cout << "Не введен размер матриц" << endl;
                    break;
                }
                cout << "Ввод матриц" << endl;
                InputMatrix(n, m, a, b);
                break;
            case 3:
            {
                if (!hasInputNM)
                {
                    cout << "Не введены матрицы" << endl;
                    break;
                }
                cout << "A + B" << endl;
                Matrix res = *a + *b;
                res.WriteMas();
            }
            break;
            case 4:
            {
                if (!hasInputNM)
                {
                    cout << "Не введены матрицы" << endl;
                    break;
                }
                cout << "A + x" << endl;
                cout << "Введите число ";
                int x;
                cin >> x;
                Matrix res = *a + x;
                res.WriteMas();
            }
            break;
            case 5:
            {
                if (!hasInputNM)
                {
                    cout << "Не введены матрицы" << endl;
                    break;
                }
                cout << "A * B" << endl;
                Matrix res = *a * *b;
                res.WriteMas();
            }
            break;
            case 6:
            {
                if (!hasInputNM)
                {
                    cout << "Не введены матрицы" << endl;
                    break;
                }
                cout << "A * x" << endl;
                cout << "Введите число ";
                int x;
                cin >> x;
                Matrix res = *a * x;
                res.WriteMas();
            }
            break;
            case 7:
            {
                if (!hasInputNM)
                {
                    cout << "Не введены матрицы" << endl;
                    break;
                }
                cout << "A++" << endl;
                ++*a;
                a->WriteMas();
            }
            break;
            case 10:
                return 0;
            }
        }
        catch (const char* error_message)
        {
            cout << error_message << endl;
        }
    }
    return 0;
}