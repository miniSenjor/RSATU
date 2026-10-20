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

int ConvertBinToInt(string &bin)
{
    int res = 0;
    for (int i = 1; i <= 8; i++)
    {
        if (bin[i] == '1')
            res += pow(2, 8 - i);
    }
    return res;
}

// Ввод A и B
void InputAB(string& a, string& b) {
    bool isANum = false, isBNum = false;
    while (!isANum || !isBNum) {
        cout << "Введите через запятую два восьмиразрядных шестнадцатеричных числа (А, В):" << endl;
        string input;
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
struct Hex8 {
    string hex, bin, m;
    int IP, Z;
    Hex8(const string& h)
    {
        hex = h;
        bin = ConvertHex8ToBin(h);
        IP = ConvertBinToInt(bin) - 127;
        Z = bin[0] - '0';
        m = bin.substr(9, 32 - 9);
        m = m.substr(0, m.find_last_of('1') + 1);
    }
};
string inv(const string& s) {
    string r = s;
    for (char& c : r) c = (c == '0') ? '1' : '0';
    return r;
}
string decToBin8(int n) {
    string s(8, '0');
    for (int i = 7; i >= 0; i--) { s[i] = '0' + n % 2; n /= 2; }
    return s;
}
string binToHex(const string& b) {
    static const char* T[16] = {
        "0000","0001","0010","0011","0100","0101","0110","0111",
        "1000","1001","1010","1011","1100","1101","1110","1111" };
    string h, p = b;
    while (p.length() % 4) p = "0" + p;
    for (size_t i = 0; i < p.length(); i += 4) {
        string n = p.substr(i, 4);
        for (int j = 0; j < 16; j++) if (T[j] == n) { h += "0123456789ABCDEF"[j]; break; }
    }
    return h;
}
string addOne(string s) {
    int i = (int)s.length() - 1;
    while (i >= 0 && s[i] == '1') { s[i] = '0'; i--; }
    if (i >= 0) s[i] = '1';
    else s = "1" + s;
    return s;
}
// Коды мантиссы: m = "0,xxxx", sign = 0/1
string cP(const string& m, int s) {
    //string f = m.substr(2);
    if (m.find('1') == string::npos) return (s ? "1.0,0" : "0.0,0");
    return string(1, '0' + s) + ".0," + m;
}
string cO(const string& m, int s) {
    //string f = m.substr(2);
    if (m.find('1') == string::npos) return (s ? "1.1,1" : "0.0,0");
    if (!s) return "0.0," + m;
    return "1.1," + inv(m);
}
string cD(const string& m, int s) {
    //string f = m.substr(2);
    if (m.find('1') == string::npos) return (s ? "не существует" : "0.0,0");
    if (!s) return "0.0," + m;
    return "1.1," + addOne(inv(m));
}
string cM(const string& m, int s) {
    //string f = m.substr(2);
    if (m.find('1') == string::npos) return (s ? "не существует" : "00.0,0");
    if (!s) return "00.0," + m;
    return "11.1," + addOne(inv(m));
}
string addMDK(const string& a, const string& b) {
    if (a == "не существует" || b == "не существует") return "не существует";
    size_t pa = a.find(','), pb = b.find(',');
    string sa = a.substr(0, 2), va = a.substr(pa + 1);
    string sb = b.substr(0, 2), vb = b.substr(pb + 1);
    size_t n = max(va.length(), vb.length());
    va += string(n - va.length(), '0');
    vb += string(n - vb.length(), '0');
    string fa = sa + va, fb = sb + vb, r = fa;
    int carry = 0;
    for (int i = (int)fa.length() - 1; i >= 0; i--) {
        int sum = (fa[i] - '0') + (fb[i] - '0') + carry;
        r[i] = '0' + sum % 2;
        carry = sum / 2;
    }
    return r.substr(0, 2) + "." + (r.substr(0, 2) == "00" ? "0" : "1") + "," + r.substr(2);
}

string trimFrac(const string& f) {
    size_t l = f.find_last_of('1');
    if (l == string::npos) return "0";
    return f.substr(0, l + 1);
}
string mdkToNormal(const string& mdk, int& sign) {
    size_t pd = mdk.find('.');
    size_t pc = mdk.find(',', pd + 1);
    string sb = mdk.substr(0, pd);
    string v = mdk.substr(pc + 1);
    if (sb == "00") { sign = 0; return "0," + v; }
    if (sb == "11") { sign = 1; return "0," + addOne(inv(v)); }
    sign = 0;
    return "0," + v;
}
void Sum(Hex8 a, Hex8 b)
{
    int IPR = max(a.IP, b.IP);
    cout << "Сложение двух чисел A + B" << endl;
    cout << "Выравненный порядок = ";
    cout << ((a.IP > b.IP) ? a.IP : b.IP) << endl;
    if (a.IP < b.IP)
    {
        string m = "0," + string(b.IP - a.IP, '0') + a.m;
        cout << "Уменьшенная мантисса mA’ = " << m << endl;
    }
    if (a.IP > b.IP)
    {
        string m = "0," + string(a.IP - b.IP, '0') + b.m;
        cout << "Уменьшенная мантисса mB’ = " << m << endl;
    }
    string mA = a.m, mB = b.m;
    size_t n = max(mA.length(), mB.length());
    mA = string(n - mA.length(), '0') + mA;
    mB = string(n - mB.length(), '0') + mB;

    int cw = 15;
    cout << setw(6) << "" << setw(cw) << "mA" << setw(cw) << "-mA"
        << setw(cw) << "mB'" << setw(cw) << "-mB'" << endl;
    cout << string(6 + cw * 4, '-') << endl;
    auto row = [&](const string& name, string(*fn)(const string&, int)) {
        cout << setw(6) << name
            << setw(cw) << fn(mA, a.Z) << setw(cw) << fn(mA, 1 - a.Z)
            << setw(cw) << fn(mB, b.Z) << setw(cw) << fn(mB, 1 - b.Z) << endl;
        };
    row("ПК", cP); row("ОК", cO); row("ДК", cD); row("МДК", cM);
    cout << endl;

    string mdkA = cM(mA, a.Z), mdkB = cM(mB, b.Z);
    if (mdkA == "не существует" || mdkB == "не существует") {
        cout << "НЕТ РЕШЕНИЯ" << endl;
        return;
    }

    cout << "  " << mdkA << endl;
    cout << "+ " << mdkB << endl;
    cout << string(mdkA.length() + 2, '-') << endl;
    string mdkR = addMDK(mdkA, mdkB);
    cout << "  " << mdkR << endl << endl;

    int ZR;
    string mR = mdkToNormal(mdkR, ZR);
    cout << "mR(МДК) = " << mdkR << endl;
    if (ZR == 0) {
        cout << "mR(ПК) = " << cP(mR, 0) << endl;
    }
    else {
        cout << "mR(ДК) = " << cD(mR, 1) << endl;
        cout << "mR(ОК) = " << cO(mR, 1) << endl;
        cout << "mR(ПК) = " << cP(mR, 1) << endl;
    }
    cout << endl;

    cout << "mR = 0," << trimFrac(mR.substr(2)) << endl;

    string mRn = mR;
    int IPn = IPR;
    bool norm = false;
    if (mRn.length() > 2 && mRn[2] == '0') {
        size_t fi = mRn.find('1', 2);
        if (fi != string::npos && fi > 2) {
            mRn = "0,1" + mRn.substr(fi + 1);
            IPn -= (int)(fi - 2);
            norm = true;
        }
    }
    if (norm) cout << "mR(норм) = 0," << trimFrac(mRn.substr(2)) << endl;

    int SPR = IPn + 127;
    string SPRb = decToBin8(SPR);
    string fm = mRn.substr(2);
    while (fm.length() < 23) fm += "0";
    string R2 = to_string(ZR) + SPRb + fm;

    cout << "ZR = " << ZR << endl;
    cout << "СПR(2) = " << SPRb << endl;
    cout << "R(2) = " << R2 << endl;
    cout << "R = " << binToHex(R2) << endl;
}
int main() {
    SetConsoleCP(866);
    SetConsoleOutputCP(866);

    SetConsoleOutputCP(1251);
    while (true) {
        string a, b;
        InputAB(a, b);

        int k;
        bool opDone = false;
        int step = 2;
        SelectMachineCode(k);
        Hex8 hexA(a);
        Hex8 hexB(b);
        cout << "ZA = " << hexA.bin[0] << " ИПА = " << hexA.IP << " mA = 0," << hexA.m << endl;
        cout << "ZB = " << hexB.bin[0] << " ИПB = " << hexB.IP << " mB = 0," << hexB.m << endl;
        switch (k)
        {
        case 1:
            Sum(hexA, hexB);
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
        //    
        //}
        //else {
        //    cout << "Операция не реализована" << endl;
        //}

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