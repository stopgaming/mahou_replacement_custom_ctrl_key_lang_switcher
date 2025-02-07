//
// Created by user on 01.02.2025.
//

#include <windows.h>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

#define WM_APP_UPDATE_ICON (WM_APP + 1)

NOTIFYICONDATA nid = {};
UINT taskbarCreatedMsg;

void
read_check_write_if_diff(BYTE DataNeeded[4], BYTE DataNeededSize, _In_opt_ LPCSTR openkey, _In_opt_ LPCSTR valuename) {

    while (1) {
        HKEY hKey;
        LONG lResult;
        BYTE data[4];
        DWORD dataSize = sizeof(data);
// Открываем ключ реестра R
        lResult = RegOpenKeyEx(HKEY_CURRENT_USER, openkey, 0, KEY_READ,
                               &hKey);
        if (lResult != ERROR_SUCCESS) {
//        std::cerr << "Ошибка открытия ключа реестра: " << lResult << std::endl;
            break;
        }
// Читаем значение Virtual Key
        lResult = RegQueryValueEx(hKey, valuename, NULL, NULL, data, &dataSize);
        if (lResult != ERROR_SUCCESS) {
//        std::cerr << "Ошибка чтения значения: " << lResult << std::endl;
            RegCloseKey(hKey);
            break;
        }
// Проверяем, равно ли значение 39 00 00 00
        if (data[0] == DataNeeded[0] && data[1] == DataNeeded[1] && data[2] == DataNeeded[2] &&
            data[3] == DataNeeded[3]) {
//        std::cout << "Значение уже равно 39 00 00 00" << std::endl;
// Закрываем ключ реестра
            RegCloseKey(hKey);
        } else {
// Закрываем ключ реестра
            RegCloseKey(hKey);
// Открываем ключ реестра RW
            lResult = RegOpenKeyEx(HKEY_CURRENT_USER, openkey, 0,
                                   KEY_READ | KEY_WRITE, &hKey);
            if (lResult != ERROR_SUCCESS) {
//        std::cerr << "Ошибка открытия ключа реестра: " << lResult << std::endl;
                break;
            }
// Устанавливаем новое значение
            lResult = RegSetValueEx(hKey, valuename, 0, REG_BINARY, DataNeeded, DataNeededSize);
            if (lResult != ERROR_SUCCESS) {
//std::cerr << "Ошибка записи значения: " << lResult <<
//std::endl;
            } else {
//std::cout << "Значение успешно изменено на 39 00 00 00" <<
//std::endl;
            }
        }
        break;
    }
}

int APIENTRY
WinMain(HINSTANCE
        hInstance,
        HINSTANCE hPrevInstance, LPSTR
        lpCmdLine,
        int nCmdShow
) {
// Инициализация GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr
    );


    {
        BYTE DataNeeded[4] = {0x39, 0x00, 0x00, 0x00};
        LPCSTR openkey = TEXT("Control Panel\\Input Method\\Hot Keys\\00000100");
        LPCSTR valuename = TEXT("Virtual Key");
        read_check_write_if_diff(DataNeeded, 4, openkey, valuename);
    }

    {
        BYTE DataNeeded[4] = {0x30, 0x00, 0x00, 0x00};
        LPCSTR openkey = TEXT("Control Panel\\Input Method\\Hot Keys\\00000101");
        LPCSTR valuename = TEXT("Virtual Key");
        read_check_write_if_diff(DataNeeded, 4, openkey, valuename);
    }



// Завершение работы GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}


#ifdef ASFSADFSADFSADF
New chat
Как на C++ проверить ключ "HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys\00000100\Virtual Key" на равенство 39 00 00 00 через чтение, и если равенства нет, то изменить это значение на 39 00 00 00?

Для работы с реестром в Windows на C++ можно использовать API функции, такие как RegOpenKeyEx, RegQueryValueEx, RegSetValueEx и RegCloseKey. Вот пример кода, который проверяет значение ключа Virtual Key и изменяет его, если оно не равно 39 00 00 00:
cpp
Copy

#include <windows.h>
#include <iostream>

int main() {
    HKEY hKey;
    LONG lResult;
    BYTE data[4];
    DWORD dataSize = sizeof(data);

    // Открываем ключ реестра
    lResult = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Control Panel\\Input Method\\Hot Keys\\00000100"), 0, KEY_READ | KEY_WRITE, &hKey);
    if (lResult != ERROR_SUCCESS) {
        std::cerr << "Ошибка открытия ключа реестра: " << lResult << std::endl;
        return 1;
    }

    // Читаем значение Virtual Key
    lResult = RegQueryValueEx(hKey, TEXT("Virtual Key"), NULL, NULL, data, &dataSize);
    if (lResult != ERROR_SUCCESS) {
        std::cerr << "Ошибка чтения значения: " << lResult << std::endl;
        RegCloseKey(hKey);
        return 1;
    }

    // Проверяем, равно ли значение 39 00 00 00
    if (data[0] == 0x39 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x00) {
        std::cout << "Значение уже равно 39 00 00 00" << std::endl;
    } else {
        // Устанавливаем новое значение
        BYTE newData[4] = {0x39, 0x00, 0x00, 0x00};
        lResult = RegSetValueEx(hKey, TEXT("Virtual Key"), 0, REG_BINARY, newData, sizeof(newData));
        if (lResult != ERROR_SUCCESS) {
            std::cerr << "Ошибка записи значения: " << lResult << std::endl;
        } else {
            std::cout << "Значение успешно изменено на 39 00 00 00" << std::endl;
        }
    }

    // Закрываем ключ реестра
    RegCloseKey(hKey);

    return 0;
}

Пояснение:

    RegOpenKeyEx: Открывает указанный ключ реестра. В данном случае, это HKEY_CURRENT_USER\Control Panel\Input Method\Hot Keys\00000100.

    RegQueryValueEx: Читает значение указанного параметра (Virtual Key).

    RegSetValueEx: Устанавливает новое значение для параметра, если текущее значение не соответствует требуемому.

    RegCloseKey: Закрывает открытый ключ реестра.

Важно:

    Программа должна быть запущена с правами администратора, так как изменение реестра может требовать повышенных привилегий.

    Будьте осторожны при работе с реестром, так как неправильные изменения могут привести к нестабильной работе системы.

Этот код проверяет значение Virtual Key и, если оно не равно 39 00 00 00, изменяет его на это значение.

#endif
