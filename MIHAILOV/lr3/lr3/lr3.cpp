//второй метод деления с восстановлением остатка и сдвигом делителя
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
        cout << "Введите два целых десятичных числа |A| < |B|:" << endl;
        cout << "A = ";
        isANum = ReadInt(a);
        cout << "B = ";
        isBNum = ReadInt(b);
        if (!isANum || !isBNum || abs(a)>=abs(b))
            cout << "Ошибка ввода исходных чисел" << endl;
    }
}

// Выбор номера операции
void SelectMachineCode(int& k) {
    k = 0;
    bool ok = false;
    while (!ok) {
        cout << "Введите номер способа деления, который необходимо реализовать:" << endl;
        cout << "1 с восстановлением остатка и сдвигом остатка" << endl;
        cout << "2 с восстановлением остатка и сдвигом делителя" << endl;
        cout << "3 без восстановления остатка и сдвигом остатка" << endl;
        cout << "4 без восстановления остатка и сдвигом делителя" << endl;
        cout << "K = ";
        if (ReadInt(k)) {
            ok = (k >= 1 && k <= 4);
        }
        if (!ok) {
            cout << "Ошибка ввода номера способа деления" << endl;
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
    
    cout << "ZR = ZA (+) ZB = " << (za+zb)%2 << endl;

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

void divideWithRestore(long A, long B, int steps) {
    long R = A;
    long Bshift = B << steps;   // делитель, сдвинутый влево на steps
    long Q = 0;                 // частное

    // ширина поля для вывода
    int width = max(bits(Bshift), max(bits(A), bits(R))) + 1;

    cout << "  Делимое  A = " << A << endl;
    cout << "  Делитель B = " << B << endl;
    cout << "  Шагов    = " << steps << endl;
    cout << "  Ширина   = " << width << " бит" << endl;
    cout << endl;

    for (int i = steps; i >= 0; i--) {
        cout << "--- Шаг " << (steps - i + 1) << " (i = " << i << ") ---" << endl;

        cout << "  " << fmt(R, width) << "   R" << endl;
        cout << "  " << fmt(Bshift, width) << "   B·2^" << i << endl;
        cout << "  " << string(width + 4, '-') << endl;

        long sub = R - Bshift;
        cout << "  " << fmt(sub, width) << "   R - B·2^" << i;
        if (sub < 0) cout << "  < 0";
        cout << endl;

        if (sub >= 0) {
            R = sub;
            Q |= (1LL << i);
            cout << "  Частный бит = 1" << endl;
        }
        else {
            cout << "  " << fmt(R, width) << "   ВО" << endl;
            cout << "  Частный бит = 0" << endl;
        }

        if (i > 0) {
            Bshift >>= 1;
            cout << "  " << fmt(Bshift, width) << "   <-" << endl;
        }
        cout << endl;
    }

    cout << "  Результат:" << endl;
    cout << "  Частное Q = " << Q << " = " << toBin(Q, steps + 1) << "(2)" << endl;
    cout << "  Остаток R = " << R << " = " << toBin(R, width) << "(2)" << endl;
    cout << "  Проверка: " << A << " = " << Q << " * " << B << " + " << R << endl;
}


int main() {
    SetConsoleCP(866);
    SetConsoleOutputCP(866);

    SetConsoleOutputCP(1251);
    while (true) {
        int a, b;
        InputAB(a, b);

        // Ширина для таблицы
        int width = max(max(bits(abs(a)), bits(abs(b))), 1) + 1;

        int k;
        bool opDone = false;
        int step = 2;
        SelectMachineCode(k);
        if (k == 2) {
            PrintTable(a, b, width);
            // Выполняем только деление с восстановлением остатка и сдвигом делителя
            cout << "Деление с восстановлением остатка и сдвигом делителя" << endl;
            while (!opDone) {

                divideWithRestore(a, b, step);
                
                if (step < width + 1)
                {
                    if (YesNo("Продолжить деление ?", "Y/N | да/нет ", "Ошибка запроса на продолжение деления"))
                        step++;
                    else
                        opDone = true;
                }
                else opDone = true;
            }
        }
        else {
            cout << "Операция не реализована" << endl;
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