#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

void PrintTime(FILETIME &fileTime) {
    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&fileTime, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    string sd = to_string(stLocal.wDay) + "."
        + to_string(stLocal.wMonth) + "."
        + to_string(stLocal.wYear) + " ";
    cout << setw(11) << sd;
    /*string st = to_string(stLocal.wSecond) + ":"
        + to_string(stLocal.wMinute) + ":"
        + to_string(stLocal.wHour) + " ";
    cout << setw(11) << st;*/
}

int CountRussianLetters(const char* buffer, DWORD size) {
    int count = 0;
    for (DWORD i = 0; i < size; ++i) {
        unsigned char c = static_cast<unsigned char>(buffer[i]);
        // В Windows-1251 русские буквы (А-Я, а-я) находятся в диапазоне 0xC0...0xFF, 
        // а также буква 'Ё' (0xA8) и 'ё' (0xB8)
        if ((c >= 0xC0 && c <= 0xFF) || c == 0xA8 || c == 0xB8) {
            count++;
        }
    }
    return count;
}

string DetectEncoding(const char* filePath) {
    HANDLE hFile = CreateFileA(
        filePath,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Не удалось открыть файл. Ошибка: " << GetLastError() << std::endl;
        return "UNKNOWN";
    }

    // Получаем размер файла
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == 0) {
        CloseHandle(hFile);
        return "WIN1251"; // Пустой файл условно считаем ANSI
    }
    // Выделяем буфер для чтения данных
    std::vector<char> buffer(fileSize);
    DWORD bytesRead = 0;

    // 2. Считываем данные с помощью ReadFile
    if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, NULL)) {
        std::wcerr << L"Ошибка чтения файла." << std::endl;
        CloseHandle(hFile);
        return "UNKNOWN";
    }

    // 3. Закрываем файл
    CloseHandle(hFile);

    // 4. Проверяем сигнатуры (BOM)
    // UTF-8 BOM: EF BB BF
    if (bytesRead >= 3 &&
        static_cast<unsigned char>(buffer[0]) == 0xEF &&
        static_cast<unsigned char>(buffer[1]) == 0xBB &&
        static_cast<unsigned char>(buffer[2]) == 0xBF) {
        return "UTF8";
    }

    // UTF-16 LE BOM: FF FE
    if (bytesRead >= 2 &&
        static_cast<unsigned char>(buffer[0]) == 0xFF &&
        static_cast<unsigned char>(buffer[1]) == 0xFE) {
        return "UTF16_LE";
    }

    // 5. Частотный анализ для OEM и Windows-1251
    // Создаем копию буфера для конвертации
    std::vector<char> convertedBuffer = buffer;

    // OemToCharBuffA переводит строку из OEM (CP866) в ANSI (Windows-1251) на месте или в другой буфер.
    // Если исходный файл был в OEM, то после этой функции в convertedBuffer появится валидный текст Windows-1251.
    OemToCharBuffA(buffer.data(), convertedBuffer.data(), bytesRead);

    // Считаем русские буквы в оригинальном буфере (предполагая, что это 1251)
    int lettersInOriginal = CountRussianLetters(buffer.data(), bytesRead);

    // Считаем русские буквы в конвертированном буфере (предполагая, что оригинал был OEM)
    int lettersInConverted = CountRussianLetters(convertedBuffer.data(), bytesRead);

    if (lettersInConverted > lettersInOriginal) {
        return "OEM";
    }
    else {
        return "WIN1251";
    }
}


int main(char* argv[], int argc)
{
    SetConsoleOutputCP(1251);
    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <путь_к_каталогу>" << std::endl;
        return 1;
    }

    string dirPath = argv[1];

    if (dirPath.empty() || dirPath.back() != '\\')
        dirPath += "\\";

    string searchPath = dirPath + "*.*";

    WIN32_FIND_DATAA findData;

    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка: Не удалось открыть каталог или каталог пуст. Код ошибки: "
            << GetLastError() << endl;
        return 1;
    }

    std::cout << "Содержимое папки " << dirPath << ":" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            cout << "[" << findData.cFileName << "]" << endl;
        }
        else
        {
            std::cout << findData.cFileName << " ";
            cout << findData.nFileSizeHigh << " ";
            cout << setw(7) << findData.nFileSizeLow << " ";
            PrintTime(findData.ftCreationTime);
            PrintTime(findData.ftLastAccessTime);
            PrintTime(findData.ftLastWriteTime);
            string name = findData.cFileName;
            if (name.substr(name.find('.'), name.length() - name.find('.')) == ".txt")
            {
                string s = argv
                
                cout << DetectEncoding(findData.cFileName);
            }
            cout << endl;
        }

    } while (FindNextFileA(hFind, &findData));
    FindClose(hFind);
}
