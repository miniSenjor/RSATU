#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    DWORD cb = 1024;
    DWORD cbNeeded = 0;
    DWORD* processIds = nullptr;
    vector<DWORD> precessIds;
    while (true)
    {
        processIds = (DWORD*)realloc(processIds, cb);
        if (!EnumProcesses(processIds, cb, &cbNeeded)) {
            std::cerr << "Не удалось получить список процессов. Ошибка: " << GetLastError() << std::endl;
            return 1;
        }
        if (cbNeeded < cb) {
            break;
        }
        cb *= 2;
    }
    DWORD processCount = cbNeeded / sizeof(DWORD);
    std::cout << "Найдено активных процессов: " << processCount << "\n" << std::endl;


    for (DWORD i = 0; i < processCount; i++) {
        DWORD pid = processIds[i];

        // Пропускаем System Idle Process (PID 0), так как его нельзя открыть с такими флагами
        if (pid == 0) continue;

        // Вызываем OpenProcess для текущего PID
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);

        if (hProcess != nullptr) {
            wchar_t szProcessName[MAX_PATH] = L"<неизвестно>";

            // Получаем имя исполняемого файла (hModule = nullptr / 0 согласно указанию)
            if (GetModuleFileNameExW(hProcess, nullptr, szProcessName, sizeof(szProcessName) / sizeof(wchar_t))) {
                std::wcout << L"PID: " << pid << L"\t Путь: " << szProcessName << std::endl;
            }
            else {
                std::wcout << L"PID: " << pid << L"\t Ошибка GetModuleFileNameEx (Код: " << GetLastError() << L")" << std::endl;
            }
            // Здесь можно выполнять действия с процессом (например, получить имя)

            // Обязательно закрываем хэндл после использования, чтобы избежать утечки ресурсов
            CloseHandle(hProcess);
        }
        else {
            // Если открыть не удалось (например, системный процесс или не хватает прав администратора)
            std::wcout << L"PID: " << pid << L"\t Ошибка OpenProcess доступ заблокирован" << std::endl;
        }
    }
    free(processIds);


    wchar_t notepad[] = L"C:\\Program Files\\Notepad++\\notepad++.exe";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (CreateProcessW(
        nullptr,       // Имя модуля (используем командную строку)
        notepad,       // Командная строка
        nullptr,       // Атрибуты безопасности процесса
        nullptr,       // Атрибуты безопасности потока
        FALSE,         // Наследование дескрипторов
        0,             // Флаги создания
        nullptr,       // Окружение (использовать окружение родителя)
        nullptr,       // Текущий диск и каталог
        &si,           // Указатель на структуру STARTUPINFO
        &pi            // Указатель на структуру PROCESS_INFORMATION
    )) {
        std::cout << "Процесс успешно запущен. Ожидание закрытия Notepad..." << std::endl;

        WaitForSingleObject(pi.hProcess, INFINITE);

        std::cout << "Notepad был закрыт" << endl;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "Ошибка вызова CreateProcess. Код: " << GetLastError() << std::endl;
    }

    UINT result = WinExec("notepad.exe", SW_SHOWMINIMIZED);

    if (result > 31) {
        std::cout << "Notepad запущен в свернутом виде" << std::endl;
    }
    else {
        std::cerr << "Ошибка вызова WinExec. Код ошибки: " << result << std::endl;
    }


    ShellExecuteA(0, "open", "C:\\Users\\skvor\\Documents\\РГАТУ\\Физика\\VM7.xlsx", NULL, NULL, SW_SHOWNORMAL);
}
