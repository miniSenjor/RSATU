#include <iostream>
#include <windows.h>
#include <string>
#include <bitset>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>
#include <iomanip>
#include <map>

using namespace std;

// Функция проверки, является ли символ шестнадцатеричной цифрой в верхнем регистре
bool isUpperHexDigit(char c) {
    // Проверяем цифры 0-9 и буквы A-F (только верхний регистр)
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

//проверка на правильный ввод 16с.с. чисел (с пробелом после запятой)
bool isValidHexPairFormatStrict(const string& input) {
    // Проверяем общую длину (8 + 1 + 1 + 8 = 18 символов)
    if (input.length() != 18) {
        return false;
    }

    // Проверяем позицию запятой (должна быть на 9-й позиции, если считать с 1)
    if (input[8] != ',') {
        return false;
    }

    if (input[9] != ' ') {
        return false;
    }

    // Проверяем первую часть (первые 8 символов)
    string first_hex = input.substr(0, 8);
    for (char c : first_hex) {
        if (!isUpperHexDigit(c)) {
            return false;
        }
    }

    // Проверяем вторую часть (последние 8 символов)
    string second_hex = input.substr(10, 8);
    for (char c : second_hex) {
        if (!isUpperHexDigit(c)) {
            return false;
        }
    }

    return true;
}

//такая же проверка на правильный ввод 16с.с. чисел, только без пробела после запятой (вряд ли будет использоваться такой вариант, но пусть будет на всякий случай )
/*bool isValidHexPairFormatStrictWithoutProbel(const string& input) {
    // Проверяем общую длину (8 + 1 + 8 = 17 символов)
    if (input.length() != 17) {
        return false;
    }

    // Проверяем позицию запятой (должна быть на 9-й позиции, если считать с 1)
    if (input[8] != ',') {
        return false;
    }

    // Проверяем первую часть (первые 8 символов)
    string first_hex = input.substr(0, 8);
    for (char c : first_hex) {
        if (!isxdigit(c)) {
            return false;
        }
    }

    // Проверяем вторую часть (последние 8 символов)
    string second_hex = input.substr(9, 8);
    for (char c : second_hex) {
        if (!isxdigit(c)) {
            return false;
        }
    }

    return true;
}*/

// Функция проверки, является ли строка целым числом
bool isInteger(const string& str) {
    if (str.empty()) return false;

    size_t start = 0;
    /*// Проверяем знак минуса в начале
    if (str[0] == '-') {
        if (str.length() == 1) return false; // Только минус - не число
        start = 1;
    }*/

    // Проверяем каждый символ на цифру
    for (size_t i = start; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// Функция преобразования строки в целое число с проверкой
bool stringToInt(const string& str, int& result) {
    if (!isInteger(str)) {
        return false;
    }

    try {
        result = stoi(str);
        return true;
    }
    catch (...) {
        return false;
    }
}

// вспомогательная функция для перевода 16с.с. -> 2с.с.
string hexCharToBinarySwitch(char hexChar) {
    hexChar = toupper(hexChar);

    switch (hexChar) {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    default: return "";
    }
}
// перевод 16-ричного числа в 2с.с.
string hexToBinarySwitch(const string& hexNumber) {
    string binaryResult = "";

    for (char hexChar : hexNumber) {
        string binaryNibble = hexCharToBinarySwitch(hexChar);
        if (binaryNibble.empty()) {
            return ""; // Некорректный символ
        }
        binaryResult += binaryNibble;
    }

    return binaryResult;
}

// Перевод простого двоичного числа в десятичное
int binaryToDecimalBitset(const string& binary) {
    if (binary.empty()) return 0;
    bitset<32> bits(binary);
    return (int)bits.to_ulong();
}

// для удаления лишних незначащих нулей справа после запятой
/*string removeTrailingZeros(const string& bin) {
    string result = bin;

    if (bin == "00000000000000000000000") {
        return "0";
    }

    while (result[result.length() - 1] == '0') {
        result = result.substr(0, result.length() - 1);
    }

    return result;
}*/

string removeTrailingZeros(const string& bin) {
    if (bin.empty()) return "";

    // Находим последнюю единицу
    size_t last_one = bin.find_last_of('1');
    if (last_one == string::npos) {
        return "0"; // Все нули
    }

    return bin.substr(0, last_one + 1);
}

// Функция преобразования десятичного числа в двоичное (без дополнения нулями)
string decimalToBinary(int n) {
    if (n == 0) {
        return "0";
    }

    string binaryString = "";
    unsigned int positive_n;

    if (n < 0) {
        positive_n = -n;
    }
    else {
        positive_n = n;
    }

    while (positive_n > 0) {
        binaryString += to_string(positive_n % 2);
        positive_n /= 2;
    }

    reverse(binaryString.begin(), binaryString.end());
    return binaryString;
}

// Функция для выравнивания двоичных чисел по максимальной длине
string alignBinary(const string& binary, size_t target_length) {
    if (binary.length() >= target_length) {
        return binary;
    }

    string aligned = binary;
    while (aligned.length() < target_length) {
        aligned = "0" + aligned;
    }
    return aligned;
}

// Функция для центрирования текста в столбце
string center(const string& text, int width) {
    if (text.length() >= width) return text.substr(0, width);

    int padding = width - text.length();
    int left_padding = padding / 2;
    int right_padding = padding - left_padding;

    return string(left_padding, ' ') + text + string(right_padding, ' ');
}

// Функция для вывода строки таблицы с центрированными значениями
/*void printTableRow(const string& label, const string& val1, const string& val2, int label_width, int column_width) {
    cout << left << setw(label_width) << label
        << center(val1, column_width)
        << center(val2, column_width)
        << endl;
}*/

// 2 variant: Функция для вывода строки таблицы с центрированными значениями
void printTableRow(const string& label, const string& val1, const string& val2, const string& val3, const string& val4, int label_width, int column_width) {
    cout << left << setw(label_width) << label
        << center(val1, column_width)
        << center(val2, column_width)
        << center(val3, column_width)
        << center(val4, column_width)
        << endl;
}

// Функция для вывода строки таблицы с центрированными значениями
void printTableRow2(const string& label, const string& val1, const string& val2, const string& val3, int label_width, int column_width) {
    cout << left << setw(label_width) << label
        << center(val1, column_width)
        << center(val2, column_width)
        << center(val3, column_width)
        << endl;
}


// Функция инверсии битов
string invers(const string& binaryStr) {
    string result = "";
    for (char c : binaryStr) {
        result += (c == '0') ? '1' : '0';
    }
    return result;
}

// Обновленный дополнительный код с защитным разрядом
string DC(const string& binary) {
    // binary уже содержит защитный разряд (например "0101" вместо "101")
    string inverted = invers(binary);

    // Вычисляем дополнительный код вручную
    string result = inverted;
    int carry = 1;
    for (int i = result.length() - 1; i >= 0 && carry > 0; i--) {
        if (result[i] == '0') {
            result[i] = '1';
            carry = 0;
        }
        else {
            result[i] = '0';
            carry = 1;
        }
    }

    return "1." + result;
}

// Обновленный модифицированный дополнительный код с защитным разрядом
string MDC(const string& binary) {
    // binary уже содержит защитный разряд
    string inverted = invers(binary);

    // Вычисляем дополнительный код вручную
    string result = inverted;
    int carry = 1;
    for (int i = result.length() - 1; i >= 0 && carry > 0; i--) {
        if (result[i] == '0') {
            result[i] = '1';
            carry = 0;
        }
        else {
            result[i] = '0';
            carry = 1;
        }
    }

    return "11." + result;
}

// Функция для сложения двоичных чисел в модифицированном дополнительном коде с защитным разрядом
string binaryAddition(const int& num1, const int& num2, size_t len) {
    int sum{ 0 };
    sum = num1 + num2;

    // Добавляем нули слева для выравнивания длины (если нужно сохранить длину)
    string decSum = decimalToBinary(sum);
    if (decSum.length() < len) {
        decSum = string(len - decSum.length(), '0') + decSum;
    }

    string result;
    if (sum < 0) {
        result = MDC(decSum);

    }
    else
        result = "00." + decSum;

    return result;
}

// Удаление незначащих нулей для целых чисел
string removeLeadingZeros(const string& number) {
    if (number.empty()) return "";

    // Находим первую цифру, не равную '0'
    size_t firstNonZero = number.find_first_not_of('0');

    if (firstNonZero == string::npos) {
        // Все символы - нули, возвращаем "0"
        return "0";
    }

    return number.substr(firstNonZero);
}

// Функция для вывода столбика умножения двоичных дробей (вряд ли пригодится, но пусть будет на всякий случай (нужно будет доработать, eсли понадобится))
/*void printBinaryMultiplicationColumn(const string& a, const string& b) {
    cout << "\nУМНОЖЕНИЕ ДВОИЧНЫХ ДРОБЕЙ В СТОЛБИК" << endl;
    cout << a << " × " << b << endl;
    cout << string(60, '=') << endl;

    // Извлекаем дробные части
    string a_frac = a.substr(2);
    string b_frac = b.substr(2);

    // Удаляем незначащие нули справа
    a_frac = a_frac.substr(0, a_frac.find_last_not_of('0') + 1);
    b_frac = b_frac.substr(0, b_frac.find_last_not_of('0') + 1);

    // Выравниваем длины для красивого вывода
    size_t max_len = max(a_frac.length(), b_frac.length());
    string a_aligned = string(max_len - a_frac.length(), '0') + a_frac;
    string b_aligned = string(max_len - b_frac.length(), '0') + b_frac;

    cout << "   0," << a_aligned << "  ← множимое" << endl;
    cout << "×  0," << b_aligned << "  ← множитель" << endl;
    cout << "   " << string(max_len + 2, '-') << endl;

    // Вектор для промежуточных произведений
    vector<string> partial_products;

    // Умножение в столбик (от младших разрядов к старшим)
    for (int i = b_aligned.length() - 1; i >= 0; i--) {
        if (b_aligned[i] == '1') {
            // Создаем частичное произведение со сдвигом
            string partial_product = a_aligned;

            // Добавляем нули справа в зависимости от позиции
            int shift = b_aligned.length() - 1 - i;
            partial_product += string(shift, '0');

            // Добавляем нули слева для выравнивания
            partial_product = string(2 * max_len - partial_product.length(), '0') + partial_product;

            partial_products.push_back(partial_product);

            // Выводим частичное произведение
            cout << "   " << partial_product;
            cout << "  ← 0," << a_aligned << " × 2^" << shift;
            cout << " (бит " << b_aligned.length() - 1 - i << " множителя = 1)" << endl;
        }
        else {
            // Пропускаем нулевые биты, но можно вывести для наглядности
            int shift = b_aligned.length() - 1 - i;
            cout << "   " << string(2 * max_len, ' ') << "  ← бит " << shift << " множителя = 0 (пропуск)" << endl;
        }
    }

    // Если есть частичные произведения, выводим сумму
    if (!partial_products.empty()) {
        cout << "   " << string(2 * max_len, '-') << endl;

        // Вычисляем итоговую сумму
        vector<int> final_result(2 * max_len, 0);

        // Суммируем все частичные произведения
        for (const string& product : partial_products) {
            int carry = 0;
            for (int j = product.length() - 1; j >= 0; j--) {
                int pos = product.length() - 1 - j;
                int sum = final_result[pos] + (product[j] - '0') + carry;
                final_result[pos] = sum % 2;
                carry = sum / 2;
            }

            // Обрабатываем оставшийся перенос
            int pos = product.length();
            while (carry > 0) {
                int sum = final_result[pos] + carry;
                final_result[pos] = sum % 2;
                carry = sum / 2;
                pos++;
            }
        }

        // Преобразуем результат в строку
        string result_binary = "";
        for (int i = final_result.size() - 1; i >= 0; i--) {
            result_binary += to_string(final_result[i]);
        }

        // Удаляем незначащие нули слева
        size_t first_one = result_binary.find('1');
        if (first_one == string::npos) {
            result_binary = "0";
        }
        else {
            result_binary = result_binary.substr(first_one);
        }

        cout << "   " << string(2 * max_len - result_binary.length(), '0') << result_binary;
        cout << "  ← промежуточная сумма" << endl;

        // Формируем окончательный результат
        string final_product = "0," + result_binary;
        string normalized = normalizeBinaryFraction(final_product);

        cout << "\nПРОМЕЖУТОЧНЫЙ РЕЗУЛЬТАТ: " << final_product << endl;
        cout << "НОРМАЛИЗОВАННЫЙ РЕЗУЛЬТАТ: " << normalized << endl;

    }
    else {
        cout << "\nРЕЗУЛЬТАТ: 0,0 (все биты множителя равны 0)" << endl;
    }
}*/

// Функция для перевода целой двоичной части в hex
string binaryIntegerToHex(const string& binaryInt) {
    if (binaryInt.empty()) return "0";

    string binary = binaryInt;
    // Удаляем незначащие нули слева
    size_t first_one = binary.find('1');
    if (first_one == string::npos) {
        return "0";
    }
    binary = binary.substr(first_one);

    // Дополняем до длины, кратной 4
    int remainder = binary.length() % 4;
    if (remainder != 0) {
        binary = string(4 - remainder, '0') + binary;
    }

    // Таблица преобразования
    map<string, char> binToHex = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
        {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
        {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
        {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}
    };

    string hex_result = "";
    for (size_t i = 0; i < binary.length(); i += 4) {
        string nibble = binary.substr(i, 4);
        hex_result += binToHex[nibble];
    }

    return hex_result;
}

// Функция для перевода дробной двоичной части в hex
string binaryFractionToHex(const string& binaryFrac) {
    if (binaryFrac.empty()) return "0";

    string fraction = binaryFrac;
    // Удаляем незначащие нули справа
    size_t last_significant = fraction.find_last_not_of('0');
    if (last_significant == string::npos) {
        return "0";
    }
    fraction = fraction.substr(0, last_significant + 1);

    // Дополняем нулями справа до длины, кратной 4
    int remainder = fraction.length() % 4;
    if (remainder != 0) {
        fraction += string(4 - remainder, '0');
    }

    // Таблица преобразования
    map<string, char> binToHex = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
        {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
        {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
        {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}
    };

    string hex_result = "";
    for (size_t i = 0; i < fraction.length(); i += 4) {
        string nibble = fraction.substr(i, 4);
        hex_result += binToHex[nibble];
    }

    return hex_result;
}

// Основная функция для перевода чисел формата "X,XXXXX" из 2 в 16 систему
string binaryFloatToHex(const string& binaryFloat) {
    // Находим позицию запятой
    size_t comma_pos = binaryFloat.find(',');

    if (comma_pos == string::npos) {
        // Нет запятой - просто целое число
        return binaryIntegerToHex(binaryFloat);
    }

    // Извлекаем целую и дробную части
    string integer_part = binaryFloat.substr(0, comma_pos);
    string fraction_part = binaryFloat.substr(comma_pos + 1);

    // Преобразуем части
    string hex_integer = binaryIntegerToHex(integer_part);
    string hex_fraction = binaryFractionToHex(fraction_part);

    // Формируем результат
    return hex_integer + "," + hex_fraction;
}

// Для IEEE 754 нужно преобразовывать отдельно компоненты
string ieee754ToHex(const string& binary32) {
    if (binary32.length() != 32) return "Некорректная длина";

    string sign = binary32.substr(0, 1);
    string exponent = binary32.substr(1, 8);
    string mantissa = binary32.substr(9);

    string hex_sign = (sign == "1") ? "-" : "";
    string hex_exponent = binaryIntegerToHex(exponent);
    string hex_mantissa = binaryIntegerToHex(mantissa);

    return hex_sign + hex_exponent + "." + hex_mantissa;
}

// Функция для преобразования двоичного числа с плавающей запятой в десятичное
double binaryFloatToDecimal(const string& binaryFloat) {
    // Ожидаемый формат: "xx.x,xxxx" или "x.xxxx" или "0,xxxx" или "1,xxxx"
    size_t dot_pos = binaryFloat.find('.');
    size_t comma_pos = binaryFloat.find(',');

    string integer_part, fractional_part;
    string sign_part = "";

    // Определяем формат числа
    if (dot_pos != string::npos && comma_pos != string::npos && dot_pos == 2) {
        // Формат "xx.x,xxxx" - со знаковыми битами
        sign_part = binaryFloat.substr(0, 2);
        integer_part = binaryFloat.substr(dot_pos + 1, comma_pos - dot_pos - 1);
        fractional_part = binaryFloat.substr(comma_pos + 1);
    }
    else if (dot_pos != string::npos) {
        // Формат "x.xxxx" - обычное представление
        integer_part = binaryFloat.substr(0, dot_pos);
        fractional_part = binaryFloat.substr(dot_pos + 1);
    }
    else if (comma_pos != string::npos) {
        // Формат "0,xxxx" или "1,xxxx" - только дробная часть с указанием целой части
        if (comma_pos == 1) {
            // Формат "x,xxxx" - одна цифра целой части
            integer_part = binaryFloat.substr(0, comma_pos);
            fractional_part = binaryFloat.substr(comma_pos + 1);
        }
        else {
            // Формат без явной целой части, считаем целую часть = 0
            integer_part = "0";
            fractional_part = binaryFloat.substr(comma_pos + 1);
        }
    }
    else {
        // Целое число
        integer_part = binaryFloat;
        fractional_part = "0";
    }

    // Преобразуем целую часть
    double integer_value = 0.0;
    if (!integer_part.empty()) {
        for (size_t i = 0; i < integer_part.length(); i++) {
            if (integer_part[i] == '1') {
                integer_value += pow(2, integer_part.length() - 1 - i);
            }
        }
    }

    // Преобразуем дробную часть
    double fractional_value = 0.0;
    if (!fractional_part.empty()) {
        for (size_t i = 0; i < fractional_part.length(); i++) {
            if (fractional_part[i] == '1') {
                fractional_value += pow(2, -((int)i + 1));
            }
        }
    }

    double result = integer_value + fractional_value;

    // Учитываем знак (если есть знаковые биты)
    if (!sign_part.empty()) {
        if (sign_part == "11") {
            result = -result;
        }
        // Если sign_part == "00" - число положительное, ничего не делаем
    }

    return result;
}

// Функция для преобразования десятичного числа в двоичное с плавающей запятой
string decimalToBinaryFloat(double decimal, int fractional_bits = 10, bool include_sign = true) {
    if (decimal == 0.0) {
        return include_sign ? "00.0,0" : "0.0";
    }

    // Определяем знак
    bool is_negative = (decimal < 0);
    decimal = abs(decimal);

    // Разделяем на целую и дробную части
    int integer_part = static_cast<int>(decimal);
    double fractional_part = decimal - integer_part;

    // Преобразуем целую часть в двоичную
    string binary_integer;
    if (integer_part == 0) {
        binary_integer = "0";
    }
    else {
        int temp = integer_part;
        while (temp > 0) {
            binary_integer = (temp % 2 == 0 ? "0" : "1") + binary_integer;
            temp /= 2;
        }
    }

    // Преобразуем дробную часть в двоичную
    string binary_fractional;
    double temp_frac = fractional_part;

    for (int i = 0; i < fractional_bits && temp_frac > 1e-10; i++) {
        temp_frac *= 2;
        if (temp_frac >= 1.0) {
            binary_fractional += "1";
            temp_frac -= 1.0;
        }
        else {
            binary_fractional += "0";
        }
    }

    // Если дробная часть пустая, добавляем ноль
    if (binary_fractional.empty()) {
        binary_fractional = "0";
    }

    // Формируем результат
    string result;
    if (include_sign) {
        string sign = is_negative ? "11" : "00";
        result = sign + "." + binary_integer + "," + binary_fractional;
    }
    else {
        result = binary_integer + "." + binary_fractional;
    }

    return result;
}

// Функция для преобразования десятичного числа в двоичное с плавающей запятой (альтернативная версия)
string decimalToBinaryFloatPrecise(double decimal, int max_fractional_bits = 20) {
    if (decimal == 0.0) {
        return "00.0,0";
    }

    // Определяем знак
    bool is_negative = (decimal < 0);
    decimal = abs(decimal);

    // Разделяем на целую и дробную части
    int integer_part = static_cast<int>(decimal);
    double fractional_part = decimal - integer_part;

    // Преобразуем целую часть в двоичную
    string binary_integer;
    if (integer_part == 0) {
        binary_integer = "0";
    }
    else {
        int temp = integer_part;
        while (temp > 0) {
            binary_integer = to_string(temp % 2) + binary_integer;
            temp /= 2;
        }
    }

    // Преобразуем дробную часть в двоичную
    string binary_fractional;
    double temp_frac = fractional_part;
    int bits_generated = 0;

    while (temp_frac > 1e-15 && bits_generated < max_fractional_bits) {
        temp_frac *= 2;
        if (temp_frac >= 1.0) {
            binary_fractional += "1";
            temp_frac -= 1.0;
        }
        else {
            binary_fractional += "0";
        }
        bits_generated++;
    }

    // Если дробная часть пустая, добавляем ноль
    if (binary_fractional.empty()) {
        binary_fractional = "0";
    }

    // Формируем результат в формате "xx.x,xxxx"
    string sign = is_negative ? "11" : "00";
    return sign + "." + binary_integer + "," + binary_fractional;
}

string shift(const string& str) {
    string result = str.substr(1, 1) + str.substr(3,1) + "." + str.substr(5, 1) + "," + str.substr(6) + "0";
    return result;
}

// суммирование двичных чисел 
string addBinaryNumbersFixedLength(const string& num1, const string& num2) {
    if (num1.length() != num2.length()) {
        return "Error: Numbers must have same length";
    }

    int n = num1.length();
    string result(n, '0');
    int carry = 0;

    for (int i = n - 1; i >= 0; i--) {
        int bit1 = num1[i] - '0';
        int bit2 = num2[i] - '0';

        int sum = bit1 + bit2 + carry;
        result[i] = (sum % 2) + '0';
        carry = sum / 2;
    }

    // Игнорируем переполнение, чтобы сохранить исходную длину
    return result;
}

// Функция для прибавления единицы к младшему разряду после запятой в двоичном числе
string addOneToBinary(const string& binary) {
    // Формат: "0,XXXX" (ноль, запятая, двоичная дробная часть)

    // Находим позицию запятой
    size_t comma_pos = binary.find(',');
    if (comma_pos == string::npos) {
        return binary; // Если нет запятой, возвращаем исходное число
    }

    // Извлекаем дробную часть
    string fraction_part = binary.substr(comma_pos + 1);

    if (fraction_part.empty()) {
        return binary; // Если нет дробной части, возвращаем исходное число
    }

    // Прибавляем 1 к младшему разряду (последнему символу)
    string result_fraction = fraction_part;
    int carry = 1; // Перенос

    // Идем справа налево (от младших разрядов к старшим)
    for (int i = result_fraction.length() - 1; i >= 0 && carry > 0; i--) {
        if (result_fraction[i] == '0') {
            result_fraction[i] = '1';
            carry = 0;
        }
        else {
            result_fraction[i] = '0';
            carry = 1;
        }
    }

    // Если остался перенос после обработки всех разрядов
    if (carry > 0) {
        // Добавляем 1 слева (это означает, что произошло переполнение в целую часть)
        result_fraction = "1" + result_fraction;
    }

    // Формируем результат
    return binary.substr(0, comma_pos + 1) + result_fraction;
}

int main()
{
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);

    while (true) {
        string a_str, b_str;
        // Шаг 1-2: Ввод и проверка чисел
        while (true) {
            string ABstr;
            cout << "Введите через запятую два восьмиразрядных шестнадцатеричных числа (А, В): ";
            getline(cin, ABstr);


            if (isValidHexPairFormatStrict(ABstr)) {
                a_str = ABstr.substr(0, 8);
                b_str = ABstr.substr(10, 8);
                break;
            }
            else {
                cout << "Ошибка ввода чисел" << endl;
            }
        }

        // Шаг 3-4: Ввод и проверка операции
        int k;
        while (true) {
            cout << "\nВведите номер выполняемой операции : " << endl;
            cout << "1  А + В" << endl;
            cout << "2  А – В" << endl;
            cout << "3  А * В" << endl;
            cout << "4  A / B" << endl;
            cout << "K = ";

            string k_str;
            getline(cin, k_str);

            if (stringToInt(k_str, k)) {
                if (k >= 1 && k <= 4) {
                    if (k == 4) {
                        break;
                    }
                    else {
                        cout << "Операция не реализована" << endl;
                        // Продолжаем цикл для повторного ввода
                    }
                }
                else {
                    cout << "Ошибка ввода номера операции" << endl;
                }
            }
            else {
                cout << "Ошибка ввода номера операции" << endl;
            }
        }

        cout << endl;

        // Шаг 5: знак, истинный порядок и мантисса
        string aBin_str, bBin_str;
        aBin_str = hexToBinarySwitch(a_str);
        bBin_str = hexToBinarySwitch(b_str);

        int ZA = 0, ZB = 0;
        if (aBin_str[0] == '1') {
            ZA = 1;
        }
        if (bBin_str[0] == '1') {
            ZB = 1;
        }

        string SPA_2, SPB_2, mA, mB;
        int SPA_10, SPB_10, IPA_10, IPB_10;

        SPA_2 = aBin_str.substr(1, 8);
        SPB_2 = bBin_str.substr(1, 8);

        SPA_10 = binaryToDecimalBitset(SPA_2);
        SPB_10 = binaryToDecimalBitset(SPB_2);

        IPA_10 = SPA_10 - 127;
        IPB_10 = SPB_10 - 127;

        mA = "0," + removeTrailingZeros(aBin_str.substr(9));
        mB = "0," + removeTrailingZeros(bBin_str.substr(9));

        cout << "ZA = " << ZA << " ИПА = " << IPA_10 << " mA = " << mA << endl;
        cout << "ZB = " << ZB << " ИПB = " << IPB_10 << " mB = " << mB << endl;

        // Шаг 6: вывод названия операции
        cout << "\nДеление двух чисел A / B" << endl;

        // Шаг 7: вывод ZR, IPR, mR
        int ZR = (ZA ^ ZB);
        cout << "\nZR = ZA (+) ZB = " << ZA << " (+) " << ZB << " = " << ZR << endl;

        string IPA_2, IPB_2;
        IPA_2 = "0" + decimalToBinary(IPA_10);
        IPB_2 = "0" + decimalToBinary(IPB_10);

        int maxLen = max(IPA_2.length(), IPB_2.length());
        IPA_2 = alignBinary(IPA_2, maxLen);
        IPB_2 = alignBinary(IPB_2, maxLen);

        cout << endl;

        //2 var ------------------------------------------------------------
        if (IPA_10 < 0) {
            cout << "ИПА(2) = -";
        }
        else {
            cout << "ИПА(2) = ";
        }
        cout << decimalToBinary(IPA_10) << endl;

        if (IPB_10 < 0) {
            cout << "ИПB(2) = -";
        }
        else {
            cout << "ИПB(2) = ";
        }
        cout << decimalToBinary(IPB_10) << endl;

        cout << endl;
        //------------------------------------------------------------------

        // вывод таблицы
        string IPA_PK, IPB_PK, IPA_OK, IPB_OK, IPA_DK, IPB_DK, IPA_MDK, IPB_MDK;
        string otr_IPA_PK, otr_IPB_PK, otr_IPA_OK, otr_IPB_OK, otr_IPA_DK, otr_IPB_DK, otr_IPA_MDK, otr_IPB_MDK;

        if (IPA_10 < 0) {
            IPA_PK = "1." + IPA_2;
            IPA_OK = "1." + invers(IPA_2);
            IPA_DK = DC(IPA_2);
            IPA_MDK = MDC(IPA_2);

            otr_IPA_PK = "0." + IPA_2;
            otr_IPA_OK = otr_IPA_PK;
            otr_IPA_DK = otr_IPA_PK;
            otr_IPA_MDK = "0" + otr_IPA_PK;
        }
        else {
            IPA_PK = "0." + IPA_2;
            IPA_OK = IPA_PK;
            IPA_DK = IPA_PK;
            IPA_MDK = "0" + IPA_PK;

            otr_IPA_PK = "1." + IPA_2;
            otr_IPA_OK = "1." + invers(IPA_2);
            otr_IPA_DK = DC(IPA_2);
            otr_IPA_MDK = MDC(IPA_2);
        }

        if (IPB_10 < 0) {
            IPB_PK = "1." + IPB_2;
            IPB_OK = "1." + invers(IPB_2);
            IPB_DK = DC(IPB_2);
            IPB_MDK = MDC(IPB_2);

            otr_IPB_PK = "0." + IPB_2;
            otr_IPB_OK = otr_IPB_PK;
            otr_IPB_DK = otr_IPB_PK;
            otr_IPB_MDK = "0" + otr_IPB_PK;
        }
        else {
            IPB_PK = "0." + IPB_2;
            IPB_OK = IPB_PK;
            IPB_DK = IPB_PK;
            IPB_MDK = "0" + IPB_PK;

            otr_IPB_PK = "1." + IPB_2;
            otr_IPB_OK = "1." + invers(IPB_2);
            otr_IPB_DK = DC(IPB_2);
            otr_IPB_MDK = MDC(IPB_2);
        }

        // Определяем максимальную ширину столбцов
        int max_width = 15;
        /*vector<string> all_values = {IPA_PK, IPB_PK,
                                      IPA_OK, IPB_OK,
                                      IPA_DK, IPB_DK,
                                     IPA_MDK, IPB_MDK };*/


                                     //2 variant------------------------------------------------------------------
        vector<string> all_values = { IPA_PK, otr_IPA_PK, IPB_PK, otr_IPB_PK,
                                      IPA_OK, otr_IPA_OK, IPB_OK, otr_IPB_OK,
                                      IPA_DK, otr_IPA_DK, IPB_DK, otr_IPB_DK,
                                     IPA_MDK, otr_IPA_MDK, IPB_MDK, otr_IPB_MDK };
        //---------------------------------------------------------------------------


        for (const auto& val : all_values) {
            if (val.length() + 2 > max_width) {
                max_width = val.length() + 2;
            }
        }

        int label_width = 6;
        int column_width = max_width;


        // Вывод таблицы
        /*cout << string(label_width, ' ')
            << center("ИПА", column_width)
            << center("ИПВ", column_width)
            << endl;*/

            //cout << string(label_width + column_width * 2, '-') << endl;


            //2 variant------------------------------------------------------------------
        cout << string(label_width, ' ')
            << center("ИПА(2)", column_width)
            << center("-ИПА(2)", column_width)
            << center("ИПВ(2)", column_width)
            << center("-ИПВ(2)", column_width)
            << endl;

        cout << string(label_width + column_width * 4, '-') << endl;
        //---------------------------------------------------------------------------


        /*printTableRow("  ПК", IPA_PK, IPB_PK, label_width, column_width);
        printTableRow("  ОК", IPA_OK, IPB_OK, label_width, column_width);
        printTableRow("  ДК", IPA_DK, IPB_DK, label_width, column_width);
        printTableRow(" МДК", IPA_MDK, IPB_MDK, label_width, column_width);*/


        //2 variant----------------------------------------------------------------------------------------
        printTableRow("  ПК", IPA_PK, otr_IPA_PK, IPB_PK, otr_IPB_PK, label_width, column_width);
        printTableRow("  ОК", IPA_OK, otr_IPA_OK, IPB_OK, otr_IPB_OK, label_width, column_width);
        printTableRow("  ДК", IPA_DK, otr_IPA_DK, IPB_DK, otr_IPB_DK, label_width, column_width);
        printTableRow(" МДК", IPA_MDK, otr_IPA_MDK, IPB_MDK, otr_IPB_MDK, label_width, column_width);
        //-------------------------------------------------------------------------------------------------

        cout << endl;

        string IPR_MDK = binaryAddition(IPA_10, (-1)*IPB_10, maxLen);
        cout << " " << IPA_MDK << endl;
        cout << "+" << endl;
        cout << " " << otr_IPB_MDK << endl;
        cout << "-----------------" << endl;
        cout << " " << IPR_MDK << endl;

        string IPR_2;
        if (IPR_MDK[0] == '0') {
            IPR_2 = removeLeadingZeros(IPR_MDK.substr(3));
        }
        else {
            IPR_2 = decimalToBinary(IPA_10 + IPB_10);
        }

        string znak = "";
        if (IPR_MDK[0] == '1') {
            znak = "-";
        }

        int IPR_10 = (IPA_10 + IPB_10);
        cout << "\nИПR(2) = " << znak << IPR_2 << endl;
        cout << "ИПR(10) = " << IPR_10 << endl;

        cout << endl;

        
        string mA_norm = "mA", mB_norm = "mB";

        // нормализация мантисс
        double mA_int = stof(mA);
        double mB_int = stof(mB);

        if (mA_int > mB_int) {
            while (mA_int > mB_int) {
                mA = mA.substr(0, 2) + "0" + mA.substr(2);
                mA_int = stof(mA);
                mA_norm = "mA'";
                if (IPR_10 < 0) {
                    IPR_10 -= 1;
                }
                else {
                    IPR_10 += 1;
                }
            }
            cout << "mA' = " << mA << endl;
            cout << "ИПR'(10) = " << IPR_10 << endl;
            cout << endl;
        }

        // выравнивание мантисс
        size_t maxLenM = max(mA.length(), mB.length());
        if (mA.length() < maxLenM) {
            mA += string(maxLenM - mA.length(), '0');
        }
        if (mB.length() < maxLenM) {
            mB += string(maxLenM - mB.length(), '0');
        }

        string mA_PK, mB_PK, otr_mB_PK, mA_OK, mB_OK, otr_mB_OK, mA_DK, mB_DK, otr_mB_DK, mA_MDK, mB_MDK, otr_mB_MDK;

        mA_PK = "0." + mA;
        mA_OK = mA_PK;
        mA_DK = mA_PK;
        mA_MDK = "0" + mA_PK;

        mB_PK = "0." + mB;
        mB_OK = mB_PK;
        mB_DK = mB_PK;
        mB_MDK = "0" + mB_PK;

        otr_mB_PK = "1." + mB;
        otr_mB_OK = "1." + invers(mB.substr(0,1)) + "," + invers(mB.substr(2, mB.length()));
        otr_mB_DK = "1." + invers(mB.substr(0, 1)) + "," + DC(mB.substr(2, mB.length())).substr(2);
        otr_mB_MDK = "11." + invers(mB.substr(0, 1)) + "," + MDC(mB.substr(2, mB.length())).substr(3);

        // Определяем максимальную ширину столбцов
        int max_width2 = 15;
        vector<string> all_valuesMantiss = {mA_PK, mB_PK, otr_mB_PK,
                                            mA_OK, mB_OK, otr_mB_OK,
                                            mA_DK, mB_DK, otr_mB_DK,
                                           mA_MDK, mB_MDK, otr_mB_MDK };


        for (const auto& val : all_values) {
            if (val.length() + 2 > max_width2) {
                max_width2 = val.length() + 2;
            }
        }

        int label_width2 = 6;
        int column_width2 = max_width2;

        string tabl_mA = "|" + mA_norm + "|";
        string tabl_mB = "|" + mB_norm + "|";
        string tabl_otr_mB = "-|" + mB_norm + "|";
        // Вывод таблицы
        cout << string(label_width2, ' ')
            << center(tabl_mA, column_width2)
            << center(tabl_mB, column_width2)
            << center(tabl_otr_mB, column_width2)
            << endl;

        cout << string(label_width2 + column_width2 * 3, '-') << endl;

        printTableRow2("  ПК", mA_PK, mB_PK, otr_mB_PK, label_width2, column_width2);
        printTableRow2("  ОК", mA_OK, mB_OK, otr_mB_OK, label_width2, column_width2);
        printTableRow2("  ДК", mA_DK, mB_DK, otr_mB_DK, label_width2, column_width2);
        printTableRow2(" МДК", mA_MDK, mB_MDK, otr_mB_MDK, label_width2, column_width2);

        cout << endl;

        cout << "Деление без восстановления остатка и сдвигом остатка" << endl;

        cout << endl;

        bool flag = true;
        int i, n = 1;
        string del1, del2, sdvig;
        string R;

        while ((flag == true) || (n == mA_MDK.length() + 1 - 5)) {
            R = "";
            
            del1 = mA_MDK;
            del2 = otr_mB_MDK;

            string res;
            double vspom_res;
            cout << " " << del1 << endl;
            cout << " " << del2 << endl;
            cout << "-------------------" << endl;

            vspom_res = binaryFloatToDecimal(del1.substr(3)) + binaryFloatToDecimal(del2.substr(3));

            res = decimalToBinaryFloat(vspom_res);
            while (res.length() < mA_MDK.length()) {
                res += "0";
            }

            if (del2[0] == '1') {
                if ((binaryFloatToDecimal(del1.substr(3)) - binaryFloatToDecimal(del2.substr(3))) < 0) {
                    res = "11." + res.substr(3);
                }
            }

            cout << " " << res;

            if (res[0] == '0') {
                cout << " >0" << endl;
                R += "1";
            }
            else {
                cout << " <0" << endl;
                R += "0";
            }

            for (i = 1; i <= n; i++) {
                //cout << "[Debug] i,n: " << i << ", " << n << endl;
                // сдвиг остатка в любом случае
                sdvig = shift(res);
                cout << " " << sdvig << " <-" << endl;
                del1 = sdvig;

                // запись +/- делителя из таблицы
                if (res[0] == '0') {
                    cout << " " << otr_mB_MDK;
                    cout << " -" << endl;
                    del2 = otr_mB_MDK;
                }
                else {
                    cout << " " << mB_MDK;
                    cout << " +" << endl;
                    del2 = mB_MDK;
                }

                cout << "-------------------" << endl;

                string del1_str = del1.substr(0,2) + del1.substr(3,1) + del1.substr(5);
                string del2_str = del2.substr(0, 2) + del2.substr(3, 1) + del2.substr(5);
                string resSumDel = addBinaryNumbersFixedLength(del1_str,del2_str);
                res = (resSumDel.substr(0, 2) + "." + resSumDel.substr(2, 1) + "," + resSumDel.substr(3));

                cout << " " << res;

                /*vspom_res = binaryFloatToDecimal(del1.substr(3)) + binaryFloatToDecimal(del2.substr(3));
                if (vspom_res >= 2) {
                    vspom_res -= 2;
                }
                //cout << "[Debug] vspom_res: " << vspom_res << endl;

                res = decimalToBinaryFloat(vspom_res);
                while (res.length() < mA_MDK.length()) {
                    res += "0";
                }
                //cout << "[Debug] res: " << res << endl;

                if (del1[0] == '1') {
                    if ((binaryFloatToDecimal(del2.substr(3)) - binaryFloatToDecimal(del1.substr(3))) < 0) {
                        res = "11." + res.substr(3);
                        //cout << "[Debug] 1) del: res = " << res << endl;
                    }
                }
                else if (del2[0] == '1') {
                    if ((binaryFloatToDecimal(del1.substr(3)) - binaryFloatToDecimal(del2.substr(3))) < 0) {
                        res = "11." + res.substr(3);
                        //cout << "[Debug] 2) del: res = " << res << endl;
                    }
                }
                else if (del1[0] == '1' && del2[0] == '1') {
                    res = "11." + res.substr(3);
                    //cout << "[Debug] 3) del: res = " << res << endl;
                }

                cout << " " << res;*/

                if (res[0] == '0') {
                    cout << " >0" << endl;
                    R += "1";
                    //cout << "[Debug] 1) R: R = " << R << endl;
                }
                else {
                    cout << " <0" << endl;
                    R += "0";
                    //cout << "[Debug] 2) R: R = " << R << endl;
                }

            }
            R = R.substr(0, 1) + "," + R.substr(1);
            cout << "\n|mR(2)| = " << R << endl;
            
            if (n == mA_MDK.length() + 1 - 5) {
                break;
            }
            else {
                string choose;
                while (true) {
                    cout << "\nПродолжить деление ?\nY/N | да/нет" << endl;
                    getline(cin, choose);
                    if (choose == "Y" || choose == "да") {
                        cout << endl;
                        n += 1;
                        break;
                    }
                    else if (choose == "N" || choose == "нет") {
                        flag = false;
                        break;
                    }
                    else {
                        cout << "Ошибка запроса на продолжение деления" << endl;
                    }
                }
            }
        } 

        string mR;
        // программа закончилась принудительно (вывод рез-та без округления)
        if (flag == false) {
            cout << endl;
            cout << "|mR2| = " << R << endl;
            R = removeTrailingZeros(R);
            cout << "mR(2) = " << (ZR == 1 ? "-" : "") << R << endl;
            cout << "mR(10) = " << (ZR == 1 ? "-" : "") <<  binaryFloatToDecimal(R) << endl;

            mR = (ZR == 1 ? "-" : "") + R;
            //проверка и нормализация мантисс
            //нормализованный - 0,1
            if (R.substr(0, 3) != "0,1") {
                cout << endl;
                mR = mR.substr(0, 2) + mR.substr(mR.find('1'), mR.length());
                if (IPR_10 < 0) {
                    IPR_10 -= 1;
                }
                else {
                    IPR_10 += 1;
                }
                cout << "mR' = " << mR << endl;
                cout << "ИПR'(10) = " << IPR_10 << endl;
            }
        }
        else {
            string ROkr = R;
            if (R[R.length() - 1] == '0') {
                ROkr = R.substr(0, R.length() - 1);
            }
            else if (R[R.length() - 1] == '1') {
                ROkr = addOneToBinary(R.substr(0, R.length() - 1));
            }

            cout << "|mR(2)|окр = " << ROkr << endl;
            
            ROkr = removeTrailingZeros(ROkr);
            cout << "mR(2) = " << (ZR == 1 ? "-" : "") << ROkr << endl;

            cout << "mR(10) = " << (ZR == 1 ? "-" : "") << binaryFloatToDecimal(ROkr) << endl;

            mR = (ZR == 1 ? "-" : "") + ROkr;
            //проверка и нормализация мантисс
            //нормализованный - 0,1
            if (ROkr.substr(0, 3) != "0,1") {
                cout << endl;
                mR = mR.substr(0, 2) + mR.substr(mR.find('1'), mR.length());
                if (IPR_10 < 0) {
                    IPR_10 -= 1;
                }
                else {
                    IPR_10 += 1;
                }
                cout << "mR' = " << mR << endl;
                cout << "ИПR'(10) = " << IPR_10 << endl;
            }
        }

        cout << endl;

        int SPR = 127 + IPR_10;
        cout << "СПR(10) = " << SPR << endl;

        string SPR_bin = decimalToBinary(SPR);
        while (SPR_bin.length() < 8) {
            SPR_bin = "0" + SPR_bin;
        }
        cout << "СПR(2) = " << SPR_bin << endl;

        cout << endl;

        string result;
        if (ZR == 1) {
            result = to_string(ZR) + SPR_bin + mR.substr(3);
        }
        else {
            result = to_string(ZR) + SPR_bin + mR.substr(2);
        }
        while (result.length() < 32) {
            result += "0";
        }
        cout << "R(2) = " << result << endl;
        cout << "R(16) = " << binaryFloatToHex(result) << endl;

        cout << endl;

        //вывод результата в шестнадцатеричном формате
        cout << "Результат = " << binaryFloatToHex(result) << endl;

        //cout << "Результат = " << ieee754ToHex(res) << endl;

        cout << endl;
        
        int f = 0;
        while (true) {
            string answer;
            cout << "Завершить программу? (Y/N) " << endl;
            getline(cin, answer);

            if (answer.length() == 1 && answer == "Y") {
                f = 1;
                break;
            }
            else if (answer.length() == 1 && answer == "N") {
                break;
            }
            else {
                cout << "Ошибка запроса на завершение программы" << endl;
            }
        }

        if (f == 1) {
            break;
        }
    }
}