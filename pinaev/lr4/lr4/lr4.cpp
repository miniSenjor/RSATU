/*определение 2 загаданных чисел по диалогу.
Один знает сумму другой произведение (эти числа не вводятся).
Ввод m n (макс и мин) n<=x, y <=m
Диалог чередование реплик S или P начинает.
Реплики вводит пользователь: знаю и не знаю.
Диалог можно прервать
Ответ
1 программа находит 1 пару чисел
2 прога находит несколько ответов
3 ответов нет вообще*/
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

long long getSum(const pair<int, int>& p) { return p.first + p.second; }
long long getProd(const pair<int, int>& p) { return 1LL * p.first * p.second; }

vector<pair<int, int>> filter(const vector<pair<int, int>>& cur, bool isSum, bool isKnow) {
    map<long long, int> freq;
    for (const auto& p : cur) {
        long long key = isSum ? getSum(p) : getProd(p);
        freq[key]++;
    }
    vector<pair<int, int>> res;
    if (isKnow)
        for (const auto& p : cur) {
            long long key = isSum ? getSum(p) : getProd(p);
            if (freq[key] == 1) res.push_back(p);
        }
    else
        for (const auto& p : cur) {
            long long key = isSum ? getSum(p) : getProd(p);
            if (freq[key] >= 2) res.push_back(p);
        }
    return res;
}

int main()
{
	setlocale(LC_ALL, "ru");
    int minVal, maxVal;
    cout << "Введите min и max: ";
    cin >> minVal >> maxVal;
    if (minVal > maxVal) swap(minVal, maxVal);

    // Начальное множество всех возможных пар (x, y), где min <= x <= y <= max
    vector<pair<int, int>> matrix;
    for (int x = minVal; x <= maxVal; ++x) {
        for (int y = x; y <= maxVal; ++y) {
            matrix.push_back({ x, y });
        }
    }
    cout << "Возможных вариантов " << matrix.size() << endl;
    cout << "Кто начинает? (S - знает сумму, P - знает произведение): ";
    char start;
    cin >> start;
    start = toupper(start);
    bool isSumTurn = (start == 'S');

    cin.ignore();

    cout << "Вводите реплики: 'know' или 'dontknow'. end - завершить.\n";

    bool dialogActive = true;
    while (dialogActive) {
        cout << (isSumTurn ? "S: " : "P: ");
        string line;
        getline(cin, line);
        if (line.empty()) break;

        string low = line;
        for (char& c : low) c = tolower(c);

        bool isSayKnow = false;
        if (low.find("dontknow") != string::npos) {
            isSayKnow = false;
        }
        else if (low.find("know") != string::npos) {
            isSayKnow = true;
        }
        else if (low.find("end") != string::npos)
        {
            dialogActive = false;
            break;
        }
        else {
            cout << "Непонятная реплика. Введите 'know' или 'dontknow'.\n";
            continue;
        }

        matrix = filter(matrix, isSumTurn, isSayKnow);
        cout << "Возможных пар: " << matrix.size() << "\n";
        isSumTurn = !isSumTurn;
    }

    cout << "\n=== Результат ===\n";
    if (matrix.empty()) {
        cout << "Ответов нет вообще.\n";
    }
    else if (matrix.size() == 1) {
        cout << "Найдена одна пара: " << matrix[0].first << " и " << matrix[0].second << "\n";
    }
    else {
        cout << "Найдено несколько возможных пар (" << matrix.size() << "):\n";
        for (const auto& p : matrix) {
            cout << p.first << " и " << p.second << "\n";
        }
    }

    return 0;
}
