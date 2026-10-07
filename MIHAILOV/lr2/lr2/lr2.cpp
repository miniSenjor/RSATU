#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Чтение целого числа из строки
bool ReadInt(int& value) {
    string s;
    if (!getline(cin, s)) return false;
    stringstream ss(s);
    ss >> value;
    if (ss.fail()) return false;
    ss >> ws;
    return ss.eof();
}

// Ввод A и B
void InputAB(int& a, int& b) {
    bool isANum = false, isBNum = false;
    while (!isANum || !isBNum) {
        a = b = 0;
        cout << "Введите два целых десятичных числа:" << endl;
        cout << "A = ";
        isANum = ReadInt(a);
        cout << "B = ";
        isBNum = ReadInt(b);
        if (!isANum || !isBNum)
            cout << "Ошибка ввода исходных чисел" << endl;
    }
}

// Выбор номера операции
void SelectMachineCode(int& k) {
    k = 0;
    bool ok = false;
    while (!ok) {
        cout << "Введите номер операции, которую необходимо выполнить:" << endl;
        cout << "1  A - B" << endl;
        cout << "2 -A - B" << endl;
        cout << "3  A + B" << endl;
        cout << "4 -A + B" << endl;
        cout << "K = ";
        if (ReadInt(k)) {
            ok = (k >= 1 && k <= 4);
        }
        if (!ok) {
            cout << "Ошибка ввода номера операции" << endl;
        }
    }
}

// Запрос Да/Нет
bool YesNo(const string& q, const string& yn, const string& er) {
    while (true) {
        cout << q << endl;
        cout << yn;
        string s;
        getline(cin, s);
        if (s == "Y" || s == "да")
            return true;
        if (s == "N" || s == "нет")
            return false;
        cout << er << endl;
    }
}

// Количество бит для представления модуля числа
int bits(int x) {
    x = abs(x);
    if (x == 0) return 1;
    int count = 0;
    while (x > 0) {
        count++;
        x >>= 1;
    }
    return count;
}

// Двоичное представление числа v в поле width бит
string ToBinary(int v, int width) {
    string s(width, '0');
    for (int i = 0; i < width; ++i) {
        if ((v >> (width - 1 - i)) & 1)
            s[i] = '1';
    }
    return s;
}

// Прямой код с заданной шириной
string ToDirectCodeW(int v, int width) {
    string mag = ToBinary(abs(v), width);
    return (v < 0 ? "1." : "0.") + mag;
}

// Обратный код с заданной шириной
string ToRevCodeW(int v, int width) {
    if (v >= 0) return ToDirectCodeW(v, width);
    string mag = ToBinary(abs(v), width);
    for (char& c : mag) c = (c == '0' ? '1' : '0');
    return "1." + mag;
}

// Дополнительный код с заданной шириной
string ToDopCodeW(int v, int width) {
    if (v >= 0) return ToDirectCodeW(v, width);
    int magVal = (1 << width) - abs(v);
    string mag = ToBinary(magVal, width);
    return "1." + mag;
}

// Модифицированный дополнительный код с заданной шириной
string ToModCodeW(int v, int width) {
    if (v >= 0) return "00." + ToBinary(abs(v), width);
    int magVal = (1 << width) - abs(v);
    string mag = ToBinary(magVal, width);
    return "11." + mag;
}

// Прямой код без знака (для вывода A(2), B(2), R(2))
string ToDirectCode(int v, bool hasAddSign) {
    string ans;
    int absV = std::abs(v);
    do {
        ans.insert(0, to_string(absV % 2));
        absV /= 2;
    } while (absV > 0);
    if (hasAddSign)
        if (v < 0)
            ans.insert(0, "1.");
        else
            ans.insert(0, "0.");
    return ans;
}

// Таблица кодов для A и B
void PrintTable(int a, int b, int width) {
    cout << "A(2) = ";
    if (a < 0) cout << "-";
    cout << ToDirectCode(a, false) << endl;

    cout << "B(2) = ";
    if (b < 0) cout << "-";
    cout << ToDirectCode(b, false) << endl;

    int colw = width + 5;
    cout << setw(colw) << "A(2)"
        << setw(colw) << "-A(2)"
        << setw(colw) << "B(2)"
        << setw(colw) << "-B(2)" << endl;

    cout << "П   "
        << setw(colw) << ToDirectCodeW(a, width)
        << setw(colw) << ToDirectCodeW(-a, width)
        << setw(colw) << ToDirectCodeW(b, width)
        << setw(colw) << ToDirectCodeW(-b, width) << endl;

    cout << "ОК  "
        << setw(colw) << ToRevCodeW(a, width)
        << setw(colw) << ToRevCodeW(-a, width)
        << setw(colw) << ToRevCodeW(b, width)
        << setw(colw) << ToRevCodeW(-b, width) << endl;

    cout << "ДК  "
        << setw(colw) << ToDopCodeW(a, width)
        << setw(colw) << ToDopCodeW(-a, width)
        << setw(colw) << ToDopCodeW(b, width)
        << setw(colw) << ToDopCodeW(-b, width) << endl;

    cout << "МДК "
        << setw(colw) << ToModCodeW(a, width)
        << setw(colw) << ToModCodeW(-a, width)
        << setw(colw) << ToModCodeW(b, width)
        << setw(colw) << ToModCodeW(-b, width) << endl;

    cout << string(colw * 4, '-') << endl;
}

int main() {
    SetConsoleCP(866);
    SetConsoleOutputCP(866);

    while (true) {
        int a, b;
        InputAB(a, b);

        // Ширина для таблицы
        int width = max(max(bits(abs(a)), bits(abs(b))), 1);
        PrintTable(a, b, width);

        int k;
        bool opDone = false;
        while (!opDone) {
            SelectMachineCode(k);
            if (k == 3) {
                // Выполняем только A + B
                int r = a + b;
                int resWidth = max(max(bits(abs(a)), bits(abs(b))), bits(abs(r))) + 1;

                cout << "A + B" << endl;

                string mdkA = ToModCodeW(a, resWidth);
                string mdkB = ToModCodeW(b, resWidth);
                string mdkR = ToModCodeW(r, resWidth);

                cout << "  " << mdkA << endl;
                cout << "+ " << mdkB << endl;
                cout << string(mdkA.length() + 2, '-') << endl;
                cout << "  " << mdkR << endl;

                cout << "[R]мдк = " << mdkR << endl;

                if (r >= 0) {
                    cout << "[R]п = " << ToDirectCodeW(r, resWidth) << endl;
                    cout << "R(2)  = " << ToDirectCode(r, false) << endl;
                    cout << "R(10) = " << r << endl;
                }
                else {
                    cout << "[R]дк = " << ToDopCodeW(r, resWidth) << endl;
                    cout << "[R]ок = " << ToRevCodeW(r, resWidth) << endl;
                    cout << "[R]п = " << ToDirectCodeW(r, resWidth) << endl;
                    cout << "R(2)  = -" << ToDirectCode(r, false) << endl;
                    cout << "R(10) = " << r << endl;
                }
                opDone = true;
            }
            else {
                cout << "Операция не реализована" << endl;
            }
        }

        // Шаг 6: новые исходные числа?
        if (YesNo("Ввести новые значения исходных чисел?",
            "Y/N | да/нет ",
            "Ошибка запроса на ввод новых значений исходных чисел")) {
            continue;
        }
        else {
            // Шаг 8: завершение программы?
            if (YesNo("Завершить программу?",
                "Y/N | да/нет ",
                "Ошибка запроса на завершение программы")) {
                break;
            }
            else {
                continue;
            }
        }
    }

    return 0;
}