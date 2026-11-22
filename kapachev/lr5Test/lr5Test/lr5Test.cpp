#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    while (true)
    {
        cout << "Введите строку для отправки: ";
        string input;
        getline(cin, input);

        HWND hWnd = FindWindowW(L"MyIPCWindow", NULL);
        if (hWnd == NULL)
        {
            cerr << "Окно с классом MyIPCWindow не найдено." << endl;
            cerr << "Убедитесь, что первое приложение запущено." << endl;
            continue;
        }

        COPYDATASTRUCT cds;
        cds.dwData = 0;                                  // произвольный идентификатор
        cds.cbData = static_cast<DWORD>(input.size() + 1); // +1 для завершающего '\0'
        cds.lpData = const_cast<char*>(input.c_str());     // указатель на ASCIIZ-строку

        SendMessageW(hWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);

        cout << "Сообщение отправлено." << endl;
    }
    return 0;
}
