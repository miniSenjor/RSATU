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

// Ввод A и B
void InputAB(int& a, int& b) {
    bool isANum = false, isBNum = false;
    while (!isANum || !isBNum || abs(a) >= abs(b)) {
        a = b = 0;
        cout << "Введите два целых десятичных числа |A| < |B|:" << endl;
        cout << "A = ";
        isANum = ReadInt(a);
        cout << "B = ";
        isBNum = ReadInt(b);
        if (!isANum || !isBNum || abs(a) >= abs(b))
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

string toMDK(long long v, int width) {
    long long mod = 1LL << width;
    long long mag = ((v % mod) + mod) % mod;   // модуль в field width
    string sign = (v < 0) ? "11" : "00";
    return sign + "." + toBin(mag, width);
}
void divideMDK(long long A, long long B, int steps, bool onlyResult = false, bool skip = false) {
    long long absA = llabs(A);
    long long absB = llabs(B);

    int width = max(bits(absA), bits(absB));

    long long R = absA;
    long long Bshift = -absB;

    bool negQ = (A < 0) != (B < 0);

    vector<int> qbits;

    if (!onlyResult) {
        cout << "  " << toMDK(R, width) << endl;
        cout << "  " << toMDK(Bshift, width) << endl;
    }

    for (int i = 0; i < steps; i++) {
        long long Rnew = R + Bshift;

        if (!onlyResult) {
            cout << "  " << string(width + 4, '-') << endl;
            cout << "  " << toMDK(Rnew, width);
            if (Rnew < 0) cout << " < 0";
            else          cout << " > 0";
            cout << endl;
        }

        int bit;
        if (Rnew < 0) {
            bit = 0;
            if (!onlyResult && i + 1 < steps)
                cout << "  " << toMDK(R, width) << " ВО" << endl;
        }
        else {
            bit = 1;
            R = Rnew;
        }
        qbits.push_back(bit);

        if (i < steps - 1) {
            Bshift >>= 1;
            if (!onlyResult)
                cout << "  " << toMDK(Bshift, width) << " <-" << endl;
        }
    }

    if (onlyResult) {
        // qbits[0] — целая часть, qbits[1..] — дробные биты
        vector<int> frac(qbits.begin() + 1, qbits.end());

        // Отбрасываем последний (защитный) бит с округлением
        if (!frac.empty()) {
            int guard = frac.back();
            frac.pop_back();
            if (guard == 1) {
                // Прибавляем 1 к младшему разряду с переносом
                int i = (int)frac.size() - 1;
                while (i >= 0 && frac[i] == 1) {
                    frac[i] = 0;
                    i--;
                }
                if (i >= 0)
                    frac[i] = 1;
                // при переполнении через всю дробь — целая часть не меняется,
                // т.к. |A| < |B| и целая часть всегда 0
            }
        }

        // Сборка строки результата
        string qstr = "0.";
        if (frac.empty())
            qstr = "0";
        for (int b : frac)
            qstr += to_string(b);

        // Значение частного как дробь
        double qval = 0;
        for (size_t i = 0; i < frac.size(); i++)
            qval += frac[i] * (1.0 / (1LL << (i + 1)));
        if (negQ) qval = -qval;
        
        if (skip)
            cout << "|R(2)|окр = ";
        else
            cout << "|R(2)| = ";
        cout << qstr << endl;
        cout << "R(2) = " << (negQ ? "-" : "") << qstr << endl;
        cout << "R(10) = " << qval << endl;
        cout << endl;
    }
}

int main() {
    SetConsoleCP(866);
    SetConsoleOutputCP(866);

    SetConsoleOutputCP(1251);
    while (true) {
        int a, b;
        InputAB(a, b);

        // Ширина для таблицы
        int width = max(max(bits(abs(a)), bits(abs(b))), 1);

        int k;
        bool opDone = false;
        int step = 2;
        SelectMachineCode(k);
        if (k == 2) {
            PrintTable(a, b, width);
            // Выполняем только деление с восстановлением остатка и сдвигом делителя
            cout << "Деление с восстановлением остатка и сдвигом делителя" << endl;
            int maxSteps = width + 1;
            while (!opDone) {

                //divideWithRestore(a, b, step);
                divideMDK(a, b, step);
                
                if (step < maxSteps) {
                    if (YesNo("Продолжить деление ?", "Y/N | да/нет ",
                        "Ошибка запроса на продолжение деления")) {
                        step++;
                    }
                    else {
                        // Пользователь сказал «нет» — пропускаем шаги и сразу ответ
                        divideMDK(a, b, maxSteps, true, true);
                        opDone = true;
                    }
                }
                else {
                    // Дошли до максимума — печатаем результат
                    divideMDK(a, b, maxSteps, true);
                    opDone = true;
                }
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