#include <iostream>
//#include <processenv.h>
//#include <winbase.h>
#include <windows.h>
#include <psapi.h>
#include <winuser.h>
using namespace std;

BOOL CALLBACK PrintWindow(HWND hwnd, LPARAM lParam)
{
	if (IsWindowVisible(hwnd))
	{
		char className[256];
		char windowTitle[256];
		GetClassNameA(hwnd, className, sizeof(className));
		GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
		LONG_PTR param = GetWindowLongPtr(hwnd, GWL_STYLE);
		cout << "hwnd - " << hwnd << endl;
		cout << "Класс - " << className << endl;
		cout << "Заголовок - " << windowTitle << endl;
		cout << "Параметры - " << param << endl;
		cout << endl;
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "RU");
	LPSTR p = GetCommandLineA();
	if (p)
		printf("Имя текущего просесса %s\n", p);
	else
		printf("Не удалось получить имя процесса");

	char buffer[32767];
	DWORD size = GetEnvironmentVariableA("Path", &buffer[0], sizeof(buffer));
	if (size)
		cout << "Окружение Path: " << buffer << endl << endl;

	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(handle, &pmc, sizeof(pmc)))
		cout << "Памяти использовано: " << pmc.WorkingSetSize / 1024 << " Кб" << endl << endl;

	EnumWindows(PrintWindow, 0);
}

