#include <iostream>
#include <string>
#include <stdexcept>
#include <windows.h>

using namespace std;

struct Node {
    int Value;
    Node* Next;
    Node(int value, Node* next = nullptr) : Value(value), Next(next) {}
};

void PrintList(Node* cur, int length)
{
    for (int i = 0; i < length; i++)
    {
        cout << cur->Value << "->";
        if (cur->Next == nullptr)
            break;
        cur = cur->Next;
    }
    cout << endl;
}

int inputInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt << endl;
        string line;
        //cin.ignore();
        //getline(cin, line);
        cin >> line;
        try {
            size_t pos;
            v = stoi(line, &pos);
            if (pos == line.length()) break;
            else cout << "Не распознанное число" << endl;
        }
        catch (...) {
            cout << "Не распознанное число" << endl;
        }
    }
    return v;
}

void add(Node*& cur, Node*& next, Node* head) {
    bool isAdd = true;
    while (isAdd) {
        int v = inputInt("Введите значение");
        cur->Value = v;
        int c = inputInt("0 - выход 1 - добавить элемент 2 - добавить цикл");
        switch (c) {
        case 0:
            isAdd = false;
            break;
        case 1:
            next = new Node(0);
            cur->Next = next;
            cur = next;
            break;
        case 2: {
            int n;
            while (true) {
                cout << "Введите номер с которым соединить элемент - ";
                string line;
                getline(cin, line);
                try {
                    n = stoi(line);
                }
                catch (...) {
                    cout << "Не распознанное число" << endl;
                    continue;
                }
                Node* node = head;
                while (node != nullptr && node->Value != n) {
                    node = node->Next;
                }
                if (node == nullptr) {
                    cout << "Не найден элемент с введенным номером" << endl;
                    continue;
                }
                if (node->Value == n) {
                    cur->Next = node;
                    break;
                }
            }
            isAdd = false;
            break;
        }
        }
    }
}

bool HasCycleNaive(Node* head, int n) {
    if (head == nullptr) return false;

    for (Node* p = head; p != nullptr; p = p->Next) {
        Node* q = p->Next;

        for (int step = 0; step < n && q != nullptr; step++) {
            if (q == p)
                return true; // вернулись в p — цикл
            q = q->Next;
        }
    }
    return false;
}

bool HasCycleFloyd(Node* head) {
    Node* slow = head;
    Node* fast = head;

    while (fast != nullptr && fast->Next != nullptr) {
        slow = slow->Next;
        fast = fast->Next;

        if (slow == fast)
            return true;

        fast = fast->Next;

        if (slow == fast)
            return true;
    }
    return false;
}

bool HasCycleByReversing2(Node* head, int length) {
    if (head == nullptr) return false;

    // ---------- Фаза 1: разворот ----------
    Node* prev = nullptr;
    Node* curr = head;
    bool hasCycle = false;

    while (curr != nullptr) {
        if (curr == head && prev != nullptr) {
            hasCycle = true;
            break;
        }

        cout << curr->Value << "->";

        Node* next = curr->Next;
        curr->Next = prev;   // разворачиваем ссылку
        prev = curr;
        curr = next;
    }
    //cout << curr->Value << "->" << endl;
    //cout << "Перевернутый список" << endl;
    //PrintList(prev, length + 1);
    // prev — последний обработанный узел. В циклическом случае это «хвост»,
    // который в исходном списке указывал на head.
    Node* tail = curr;

    // ---------- Фаза 2: восстановление ----------
    Node* restorePrev = nullptr;
    Node* restoreCurr = tail;

    while (restoreCurr != nullptr) {
        Node* next = restoreCurr->Next;
        restoreCurr->Next = restorePrev;
        restorePrev = restoreCurr;
        restoreCurr = next;
    }

    // Если был цикл, нужно замкнуть хвост обратно на голову
    if (hasCycle && tail != nullptr) {
        tail->Next = head;
    }
    PrintList(head, length + 1);

    return hasCycle;
}

bool HasCycleByReversing(Node* head, int length) {
    if (head == nullptr) return false;

    Node* headNext = head->Next;

    // разворот
    Node* prev = nullptr;
    Node* curr = head;
    bool hasCycle = false;

    cout << "Переворот списка" << endl;
    while (curr != nullptr) {
        if (curr == head && prev != nullptr) {
            hasCycle = true;
            break;
        }

        cout << curr->Value << "->";

        Node* next = curr->Next;
        curr->Next = prev;
        prev = curr;
        curr = next;
    }

    if (curr != nullptr)
        cout << curr->Value << "->" << endl;
    Node* tail = prev;

    // восстановление
    bool headVisited = false;
    Node* restorePrev = nullptr;
    Node* restoreCurr = tail;

    while (restoreCurr != nullptr) {
        if (restoreCurr == head) headVisited = true;

        Node* next = restoreCurr->Next;
        restoreCurr->Next = restorePrev;
        restorePrev = restoreCurr;
        restoreCurr = next;
    }

    if (hasCycle) {
        if (headVisited) {
            if (tail != nullptr) tail->Next = head;
        }
        else {
            head->Next = headNext;
        }
    }

    cout << "Восстановленный список" << endl;
    PrintList(head, length + 3);

    return hasCycle;
}



void AddNode(Node* &cur)
{
    Node *next = new Node(cur->Value+1);
    cur->Next = next;
    cur = next;
}


void CreateCycle(Node* head, Node* last, int num)
{
    for (int i = 0; i <= num; i++)
    {
        if (i == num)
        {
            last->Next = head;
            return;
        }
        if (head->Next == nullptr)
        {
            cout << "Список слишком короткий" << endl;
            return;
        }
        head = head->Next;
    }
}

void IsCycle(bool hasCycle)
{
    if (hasCycle)
        cout << "Цикл есть" << endl;
    else
        cout << "Цикла нет" << endl;
}

int main() {
    SetConsoleOutputCP(1251);
    cout << "Заполнение списка" << endl;
    cout << "Введите кол-во элементов = ";
    int length;
    cin >> length;
    Node *head = new Node(0), * cur = head;
    for (int i = 0; i < length; i++)
    {
        AddNode(cur);
    }
    while (true)
    {
        int p = inputInt("Выберите команду\r\n1 - простой перебор 2 - два указателя 3 - проверка переворачиванием 4 - вывод списка 5 - переопределить последнюю ссылку");
        bool hasCycle = false;
        switch (p) {
        case 1:
            hasCycle = HasCycleNaive(head, length+1);
            IsCycle(hasCycle);
            break;
        case 2:
            hasCycle = HasCycleFloyd(head);
            IsCycle(hasCycle);
            break;
        case 3:
            hasCycle = HasCycleByReversing(head, length);
            IsCycle(hasCycle);
            break;
        case 4:
            PrintList(head, length+3);
            break;
        case 5:
            int num = inputInt("Введите номер на который будет ссылаться последний элемент");
            CreateCycle(head, cur, num);
            break;
        }
    }
    return 0;
}