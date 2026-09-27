#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>

using namespace std;

bool ReadInt(int& value) {
    string s;
    if (!getline(cin, s)) return false;
    stringstream ss(s);
    ss >> value;
    if (ss.fail()) return false;
    ss >> ws;
    return ss.eof();
}

void InputAB(int& a, int& b) {
    bool isANum = false, isBNum = false;
    while (!isANum || !isBNum) {
        a = b = 0;
        cout << "Введите два целых десятичных чисела: " << endl;
        cout << "A = ";
        isANum = ReadInt(a);

        cout << "B = ";
        isBNum = ReadInt(b);

        if (!isANum || !isBNum)
            cout << "Ошибка ввода исходных чисел" << endl;

    }
}

void InputC(int a, int b, int& c, bool& isNegZero) {
    c = 0;
    isNegZero = false;
    bool ok = false;
    while (!ok) {
        cout << "Введите целое десятичное число С, принадлежащее заданному диапазону:" << endl;
        cout << "C = ";

        string s;
        getline(cin, s);
        if (s == "-0") {
            isNegZero = true;
            c = 0;
            ok = (c >= a && c <= b);
            if (!ok) {
                cout << "Ошибка ввода числа С" << endl;
                isNegZero = false;
            }
        }
        else {
            stringstream ss(s);
            if (ss >> c) {
                ss >> ws;
                if (ss.eof()) {
                    ok = (c >= a) && (c <= b);
                }
            }
            if (!ok) {
                cout << "Ошибка ввода числа С" << endl;
            }
        }
    }
}

void SelectMachineCode(int& k) {
    k = 0;
    bool ok = false;
    while (!ok) {
        cout << "Введите номер операции, которую необходимо выполнить:" << endl;
        cout << "1 A-B" << endl;
        cout << "2-A-B" << endl;
        cout << "3 A+B" << endl;//мой вариант
        cout << "4-A+B" << endl;
        cout << "K = ";
        //может быть не заданная операция
        if (ReadInt(k)) {
            ok = (k > 0) && (k < 5);
        }
        if (!ok) {
            cout << "Ошибка ввода номера операции" << endl;
        }
    }
}

bool YesNo(const string& q, const string& yn, const string& er) {
    while (true) {
        cout << q << endl;
        cout << yn;
        string s;
        getline(cin, s);
        //for (char& c : s) c = tolower(c);
        if (s == "Y" || s == "да")
            return true;
        if (s == "N" || s == "нет")
            return false;
        cout << er << endl;
    }
}

string ToDirectCode(int v, bool hasAddSign) {
    string ans;
    int absV = std::abs(v);
    do {
        ans.insert(0, to_string(absV % 2));
        absV /= 2;
    } while (absV > 0);
    if(hasAddSign)
        if (v < 0)
            ans.insert(0, "1.");
        else
            ans.insert(0, "0.");
    return ans;
}

string ToRevCode(int v, bool hasAddSign) {
    string ans;
    int absV = std::abs(v);
    if (v < 0) {
        do {
            ans.insert(0, to_string((absV % 2 + 1) % 2));
            absV /= 2;
        } while (absV > 0);
        if(hasAddSign)
            ans.insert(0, "1.");
    }
    else {
        ans = ToDirectCode(v, hasAddSign);
    }
    return ans;
}

string ToDopCode(int v, bool hasAddSign) {
    string revCode;
    if (v < 0) {
        revCode = ToRevCode(v, hasAddSign);
        for (int i = revCode.length() - 1; i >= 0; i--) {
            if (revCode[i] == '0') {
                revCode[i] = '1';
                break;
            }
            else if (revCode[i] == '1') {
                revCode[i] = '0';
            }
        }
    }
    else {
        revCode = ToDirectCode(v, hasAddSign);
    }
    return revCode;
}

string ToModCode(int v, bool hasAddSign) {
    string ans = ToDopCode(v, hasAddSign);
    if (hasAddSign)
        ans = ans[0] + ans;
    return ans;
}

void PrintTable(int a, int b)
{
    cout << "A(2) = ";
    if (a < 0)
        cout << "-";
    string doubleA = ToDirectCode(a, false);
    cout << doubleA << endl;
    cout << "B(2) = ";
    if (b < 0)
        cout << "-";
    string doubleB = ToDirectCode(b, false);
    cout << doubleB << endl;
    
    
    int len = max(doubleA.length(), doubleB.length())+2;
    cout << "    A(2)";
    for (int i = 2; i < len; i++)
        cout << " ";
    cout << "-A(2)";
    for (int i = 2; i < len; i++)
        cout << " ";
    cout << "B(2)";
    for (int i = 2; i < len; i++)
        cout << " ";
    cout << "-B(2)";
    for (int i = 2; i < len; i++)
        cout << " ";
    cout << endl;
    cout << "П   " << ToDirectCode(a, true) << "  " << ToDirectCode(-a, true) << "  " << ToDirectCode(b, true) << "  " << ToDirectCode(-b, true) << endl;
    for (int i = 0; i < len * 5; i++)
        cout << "-";
    cout << endl;
    cout << "ОК  " << ToRevCode(a, true) << "  " << ToRevCode(-a, true) << "  " << ToRevCode(b, true) << "  " << ToRevCode(-b, true) << endl;
    for (int i = 0; i < len * 4 + 4; i++)
        cout << "-";
    cout << endl;
    cout << "ДК  " << ToDopCode(a, true) << "  " << ToDopCode(-a, true) << "  " << ToDopCode(b, true) << "  " << ToDopCode(-b, true) << endl;
    for (int i = 0; i < len * 4 + 4; i++)
        cout << "-";
    cout << endl;
    cout << "МДК " << ToModCode(a, true) << " " << ToModCode(-a, true) << " " << ToModCode(b, true) << " " << ToModCode(-b, true) << endl;
    for (int i = 0; i < len * 4 + 4; i++)
        cout << "-";
    cout << endl;


}

int main() {
    SetConsoleCP(866);
    SetConsoleOutputCP(866);
    bool isInputCodeNumber = true, isInputC = true, isInputLimit = true, isInputExit = false;
    while (!isInputExit) {
        isInputLimit = true;
        while (isInputLimit) {
            int a, b, c, k;
            InputAB(a, b);
            PrintTable(a, b);
            isInputC = true;
            while (isInputC) {
                bool isNegZero = false;
                InputC(a, b, c, isNegZero);
                isInputCodeNumber = true;
                while (isInputCodeNumber) {
                    SelectMachineCode(k);
                    string code;
                    bool special = false;
                    switch (k) {
                    case 1:
                        cout << "Прямой код" << endl;
                        if (isNegZero)
                            code = "1.0";
                        else
                            code = ToDirectCode(c, true);
                        break;
                    case 2:
                        cout << "Обратный код" << endl;
                        if (isNegZero)
                            code = "1.1";
                        else
                            code = ToRevCode(c, true);
                        break;
                    case 3:
                        cout << "Дополнительный код" << endl;
                        if (isNegZero)
                        {
                            special = true;
                            cout << "для -0 не существует" << endl;
                        }
                        else
                            code = ToDopCode(c, true);
                        break;
                    default:
                        cout << "Модифицированный дополнительный код" << endl;
                        if (isNegZero)
                        {
                            special = true;
                            cout << "для -0 не существует" << endl;
                        }
                        else
                            code = ToModCode(c, true);
                        break;
                    }
                    if (!special)
                        cout << "C=" << code << endl;
                    isInputCodeNumber = YesNo("Выбрать новый номер машинного кода?", "Y/N | да/нет ", "Ошибка запроса на ввод нового номера машинного кода");
                }
                isInputC = YesNo("Ввести новое значение числа С?", "Y/N | да/нет ", "Ошибка запроса на ввод нового значения числа С");
            }
            isInputLimit = YesNo("Ввести новые границы диапазона?", "Y/N | да/нет ", "Ошибка запроса на ввод новых границ диапазона");
        }
        isInputExit = YesNo("Завершить программу?", "Y/N | да/нет ", "Ошибка запроса на завершение программы");
    }
    return 0;
}