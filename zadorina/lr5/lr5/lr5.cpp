#include <iostream>
#include <windows.h>
#include <string>
#include "Array.h"

using namespace std;

struct complex {
    double re, im;
    friend complex operator*(const complex& x, const complex& y){
        complex c;
        c.re = x.re * y.re - x.im * y.im;
        c.im = x.re * y.im + x.im * y.re;
        return c;
    }
    friend bool operator==(const complex& x, const complex& y) {
        if (x.re == y.re && x.im == y.im)
            return true;
        else
            return false;
    }
    friend ostream& operator<<(std::ostream& os, const complex& c) {
        os << '(' << c.re << (c.im >= 0 ? "+" : "") << c.im << "i)";
        return os;
    }

    friend bool operator!=(const complex& a, const complex& b) {
        return !(a == b);
    }
};

Array<complex> InputArray(const string& name) {
    int n;
    cout << "Введите кол-во элементов массива " << name << " = ";
    cin >> n;
    if (n <= 0) {
        cout << "Количество должно быть положительным. Создаётся пустой массив.\n";
        return Array<complex>(0);
    }

    complex* ar = new complex[n];
    for (int i = 0; i < n; ++i) {
        cout << "Введите комплексное число " << i + 1 << " (re im): ";
        cin >> ar[i].re >> ar[i].im;
    }

    Array<complex> result(ar, n);
    delete[] ar;
    return result;
}

int main() {
    SetConsoleOutputCP(1251);
    int intNums1[] = { 1, 2, 3, 4, 5 };
    int intNums2[] = { 5, 4, 3, 2, 1 };
    float fNums1[] = { 1.2f, 3.5f, 7.9f, 9.0f, 11.1f };
    float fNums2[] = { 2.0f, 0.1f, 5.0f, 6.1f, 8.4f };
    Array<int> intArray1(intNums1);
    Array<int> intArray2(intNums2);
    Array<float> fArray1(fNums1);
    Array<float> fArray2(fNums2);

    fArray1.Print();
    fArray2.Print();
    if (fArray1 == fArray2)
        cout << "Массивы равны" << endl;
    else
        cout << "Массивы не равны" << endl;

    Array<int> res = intArray1 * intArray2;
    intArray1.Print();
    cout << "*" << endl;
    intArray2.Print();
    cout << "=" << endl;
    res.Print();

    Array<complex> a1 = InputArray("1");
    Array<complex> a2 = InputArray("2");

    while (true) {
        cout << " 1 - Сравнить массивы\n"
            << " 2 - Умножить массивы поэлементно\n"
            << " 3 - Ввести новые значения массива 1\n"
            << " 4 - Ввести новые значения массива 2\n"
            << " 5 - Вывести массивы\n"
            << " 0 - Выход\n"
            << "Выбор: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cout << "Введите число.\n";
            continue;
        }

        try {
            switch (choice) {
            case 1:
                if (a1 == a2)
                    cout << "Массивы равны\n";
                else
                    cout << "Массивы не равны\n";
                break;

            case 2: {
                Array<complex> c = a1 * a2;
                cout << "Результат: ";
                c.Print();
                break;
            }

            case 3:
                a1 = InputArray("1");
                break;

            case 4:
                a2 = InputArray("2");
                break;

            case 5:
                cout << "Массив 1: "; a1.Print();
                cout << "Массив 2: "; a2.Print();
                break;

            case 0:
                return 0;

            default:
                cout << "Неизвестная команда\n";
            }
        }
        catch (const char* e) {
            cout << "Ошибка: " << e << "\n";
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
    
    return 0;
}