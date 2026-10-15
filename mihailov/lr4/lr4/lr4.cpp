//второй метод деления с восстановлением остатка и сдвигом делителя
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <vector>
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

bool CheckHex8(string& s)
{
    if (s.length() != 8)
        return false;
    for (char c : s)
    {
        if ((s[0] < 'A' || s[0] > 'F') && (s[0] < '0' && s[0] > '9'))
            return false;
    }
    return true;
}

// Ввод A и B
void InputAB(string& a, string& b) {
    bool isANum = false, isBNum = false;
    while (!isANum || !isBNum) {
        cout << "Введите через запятую два восьмиразрядных шестнадцатеричных числа (А, В):" << endl;
        string input, a, b;
        getline(cin, input);
        int startB = (int)input.find(", ", 0);
        a = input.substr(0, startB);
        b = input.substr(startB + 2, input.length() - startB - 2);
        isANum = CheckHex8(a);
        isBNum = CheckHex8(b);
        if (!isANum || !isBNum)
            cout << "Ошибка ввода чисел" << endl;
    }
}

// Выбор номера операции
void SelectMachineCode(int& k) {
    k = 0;
    bool ok = false;
    while (!ok) {
        cout << "Введите номер выполняемой операции:" << endl;
        cout << "A + B" << endl;
        cout << "A - B" << endl;
        cout << "A * B" << endl;
        cout << "A / B" << endl;
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
    cout << ToDirectCode(a, false) << endl;

    cout << "B(2) = ";
    if (b < 0) cout << "-";
    cout << ToDirectCode(b, false) << endl;

    int za = 0;
    cout << "ZA = ";
    if (a < 0) {
        cout << "1";
        za = 1;
    }
    else
        cout << "0";
    cout << endl;

    int zb = 0;
    cout << "ZB = ";
    if (b < 0) {
        cout << "1";
        zb = 1;
    }
    else
        cout << "0";
    cout << endl;

    cout << "ZR = ZA (+) ZB = " << (za + zb) % 2 << endl;

    int absA = abs(a), absB = abs(b);

    int colw = width + 5;
    cout << setw(colw) << "|A(2)|"
        << setw(colw) << "|B(2)|"
        << setw(colw) << "-|B(2)|" << endl;
    cout << string(colw * 4 + 4, '-') << endl;

    cout << "П   "
        << setw(colw) << ToDirectCodeW(absA, width)
        << setw(colw) << ToDirectCodeW(absB, width)
        << setw(colw) << ToDirectCodeW(-absB, width) << endl;
    cout << string(colw * 4 + 4, '-') << endl;

    cout << "ОК  "
        << setw(colw) << ToRevCodeW(absA, width)
        << setw(colw) << ToRevCodeW(absB, width)
        << setw(colw) << ToRevCodeW(-absB, width) << endl;
    cout << string(colw * 4 + 4, '-') << endl;

    cout << "ДК  "
        << setw(colw) << ToDopCodeW(absA, width)
        << setw(colw) << ToDopCodeW(absB, width)
        << setw(colw) << ToDopCodeW(-absB, width) << endl;
    cout << string(colw * 4 + 4, '-') << endl;

    cout << "МДК "
        << setw(colw) << ToModCodeW(absA, width)
        << setw(colw) << ToModCodeW(absB, width)
        << setw(colw) << ToModCodeW(-absB, width) << endl;

    cout << string(colw * 4 + 4, '-') << endl;
}

// Двоичное представление числа v в поле width бит
string toBin(long long v, int width) {
    string s(width, '0');
    for (int i = 0; i < width; i++) {
        if ((v >> (width - 1 - i)) & 1) s[i] = '1';
    }
    return s;
}

// Формат "s.xxxxx" — знак, точка, двоичные разряды
string fmt(long long v, int width) {
    string sign = (v < 0) ? "1" : "0";
    return sign + "." + toBin(llabs(v), width);
}

string toMDK(long long v, int width) {
    long long mod = 1LL << width;
    long long mag = ((v % mod) + mod) % mod;   // модуль в field width
    string sign = (v < 0) ? "11" : "00";
    return sign + "." + toBin(mag, width);
}

string ConvertHex8ToBin(const string &hex)
{
    static const char* table[16] = {
        "0000","0001","0010","0011",
        "0100","0101","0110","0111",
        "1000","1001","1010","1011",
        "1100","1101","1110","1111"
    };
    string bin;
    bin.reserve(hex.size() * 4);
    for (char c : hex) {
        int v;
        if (c >= '0' && c <= '9')      v = c - '0';
        else if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
        else continue;
        bin += table[v];
    }
    return bin;
}

int main() {
    SetConsoleCP(866);
    SetConsoleOutputCP(866);

    SetConsoleOutputCP(1251);
    while (true) {
        string a, b;
        InputAB(a, b);

        // Ширина для таблицы
        //int width = max(max(bits(abs(a)), bits(abs(b))), 1);

        int k;
        bool opDone = false;
        int step = 2;
        SelectMachineCode(k);
        string binA = ConvertHex8ToBin(a);
        string binB = ConvertHex8ToBin(b);
        cout << "ZA = " << binA[0] << " ИПА = ";
        switch (k)
        {
        case 1:
            break;
        case 2:
            cout << "Операция не реализована" << endl;
            break;
        case 3:
            cout << "Операция не реализована" << endl;
            break;
        case 4:
            cout << "Операция не реализована" << endl;
            break;
        }
        //if (k == 1) {
        //    PrintTable(a, b, width);
        //    // Выполняем только деление с восстановлением остатка и сдвигом делителя
        //    cout << "Деление с восстановлением остатка и сдвигом делителя" << endl;
        //    int maxSteps = width + 1;
        //    while (!opDone) {

        //        //divideWithRestore(a, b, step);
        //        divideMDK(a, b, step);

        //        if (step < maxSteps) {
        //            if (YesNo("Продолжить деление ?", "Y/N | да/нет ",
        //                "Ошибка запроса на продолжение деления")) {
        //                step++;
        //            }
        //            else {
        //                // Пользователь сказал «нет» — пропускаем шаги и сразу ответ
        //                divideMDK(a, b, maxSteps, true, true);
        //                opDone = true;
        //            }
        //        }
        //        else {
        //            // Дошли до максимума — печатаем результат
        //            divideMDK(a, b, maxSteps, true);
        //            opDone = true;
        //        }
        //    }
        //}
        //else {
        //    cout << "Операция не реализована" << endl;
        //}

        // Шаг 6: новые исходные числа?
        
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

    return 0;
}