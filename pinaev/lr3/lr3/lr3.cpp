#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <windows.h>
using namespace std;
//A+(B*(C-D)+E)/F

float getTop(stack<float> &s)
{
    float t = s.top();
    s.pop();
    return t;
}

int getPrecedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

int main() {
    SetConsoleOutputCP(1251);
    map<char, float> alphabet;
    string infix;
    cout << "Введите выражение в инфиксной форме" << endl;
    getline(cin, infix);

    stack<char> st;
    string postfix;
    int countChar = 0, countOper = 0, countOpenSkob = 0;

    for (char c : infix) {
        if (isalpha(c)) {
            postfix += c;
            alphabet[c] = 0;
            postfix += ' ';
            if (countChar++ != countOper)
            {
                cout << "Некорректное выражение";
                return 0;
            }
            
        }
        else if (c == '(') {
            if (countChar != countOper)
            {
                cout << "Некорректное выражение";
                return 0;
            }
            countOpenSkob++;
            st.push(c);
        }
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                postfix += st.top();
                postfix += ' ';
                st.pop();
            }
            if (--countOpenSkob < 0)
            {
                cout << "Некорректная запись";
                return 0;
            }
            if (!st.empty()) st.pop();
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!st.empty() && st.top() != '(' &&
                getPrecedence(st.top()) >= getPrecedence(c)) {
                postfix += st.top();
                postfix += ' ';
                st.pop();
            }
            st.push(c);
            if (++countOper != countChar)
            {
                cout << "Некорректное выражение";
                return 0;
            }

        }
    }

    while (!st.empty()) {
        postfix += st.top();
        postfix += ' ';
        st.pop();
    }

    if (!postfix.empty()) postfix.pop_back();
    if (countChar - 1 != countOper || countOpenSkob!=0)
    {
        cout << "Некоректное выражение";
        return 0;
    }
    cout << postfix << endl;

    cout << "Введите значение букв" << endl;
    for (auto& pair : alphabet) {
        cout << pair.first << "=";
        cin >> pair.second;
    }

    stack<float> stPost;
    for (char c : postfix)
    {
        switch (c)
        {
            float res;
        case '+':
            res = getTop(stPost) + getTop(stPost);
            stPost.push(res);
            break;
        case '-':
            res = getTop(stPost) - getTop(stPost);
            stPost.push(res);
            break;
        case '*':
            res = getTop(stPost) * getTop(stPost);
            stPost.push(res);
            break;
        case '/':
            res = getTop(stPost) / getTop(stPost);
            stPost.push(res);
            break;
        default:
            if(c!=' ')
                stPost.push(alphabet[c]);
            break;
        }
    }
    cout << stPost.top();
    return 0;
}
