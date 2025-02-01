//
// Created by user on 01.02.2025.
//

#include <windows.h>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

extern void main_step(void);
static int state_key_after_send_old=0;
extern int state_key_after_send;

extern void updateicon2(void);

DWORD WINAPI MyThreadFunction( LPVOID lpParam )
{
    while (1) {
        main_step();
        if (state_key_after_send_old != state_key_after_send)
        {
            updateicon2();
            state_key_after_send_old = state_key_after_send;
        }
    }
}

#define WM_APP_UPDATE_ICON (WM_APP + 1)

NOTIFYICONDATA nid = {};
HWND hwnd;
UINT taskbarCreatedMsg;
HICON hIcon = nullptr;

void UpdateIcon(char letter) {
    if (hIcon) {
        DestroyIcon(hIcon);
    }

    // Создаем временный контекст устройства
    HDC hdc = CreateCompatibleDC(nullptr);
    if (!hdc) return;

    // Создаем битмап для иконки
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 16, 16);
    if (!hBitmap) {
        DeleteDC(hdc);
        return;
    }

    SelectObject(hdc, hBitmap);

    // Очищаем фон
    RECT rect = {0, 0, 16, 16};
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // Рисуем букву
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, &letter, 1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Создаем иконку из битмапа
    ICONINFO iconInfo = {0};
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hBitmap;
    hIcon = CreateIconIndirect(&iconInfo);

    // Освобождаем ресурсы
    DeleteObject(hBitmap);
    DeleteDC(hdc);

    // Обновляем иконку в панели задач
    nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

HANDLE  hThread;

static char currentLetter = '0';
void updateicon2(void) {
    // Обновляем иконку в зависимости от переменной
    if (state_key_after_send == 1)
        currentLetter = 'E';
    if (state_key_after_send == 2)
        currentLetter = 'R';
    UpdateIcon(currentLetter);
    //currentLetter = (currentLetter == 'A') ? 'B' : 'A';
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // Инициализируем структуру NOTIFYICONDATA
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_APP_UPDATE_ICON;
            nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            Shell_NotifyIcon(NIM_ADD, &nid);

            // Обновляем иконку с начальной буквой
            UpdateIcon('0');

            hThread = CreateThread(
                    NULL,                   // default security attributes
                    0,                      // use default stack size
                    MyThreadFunction,       // thread function name
                    0,          // argument to thread function
                    0,                      // use default creation flags
                    NULL);   // returns the thread identifier


            break;

        case WM_APP_UPDATE_ICON: {
            updateicon2();
        }
            break;

        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Регистрируем класс окна
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TaskbarIconApp";
    RegisterClassEx(&wc);

    // Создаем окно
    hwnd = CreateWindowEx(0, "TaskbarIconApp", "Key Lang Switcher Icon App and ctrl App", 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Завершение работы GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}



#ifdef ASFSADFSADFSADF

New chat
Today
New chat
New chat
Get App
New chat
Как в Windows на C++ добавить в панель задач иконку, где просто рисовать одну букву в зависимости от некоторой переменной?

Для добавления иконки в панель задач Windows на C++ и отображения на ней одной буквы в зависимости от значения переменной, можно использовать API Windows. Основные шаги включают создание иконки с нужной буквой и её обновление в зависимости от изменения переменной.

Вот пример кода, который демонстрирует, как это можно сделать:
cpp
Copy

#include <windows.h>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

#define WM_APP_UPDATE_ICON (WM_APP + 1)

NOTIFYICONDATA nid = {};
HWND hwnd;
UINT taskbarCreatedMsg;
HICON hIcon = nullptr;

void UpdateIcon(char letter) {
    if (hIcon) {
        DestroyIcon(hIcon);
    }

    // Создаем временный контекст устройства
    HDC hdc = CreateCompatibleDC(nullptr);
    if (!hdc) return;

    // Создаем битмап для иконки
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 16, 16);
    if (!hBitmap) {
        DeleteDC(hdc);
        return;
    }

    SelectObject(hdc, hBitmap);

    // Очищаем фон
    RECT rect = {0, 0, 16, 16};
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // Рисуем букву
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, &letter, 1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Создаем иконку из битмапа
    ICONINFO iconInfo = {0};
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hBitmap;
    hIcon = CreateIconIndirect(&iconInfo);

    // Освобождаем ресурсы
    DeleteObject(hBitmap);
    DeleteDC(hdc);

    // Обновляем иконку в панели задач
    nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // Инициализируем структуру NOTIFYICONDATA
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_APP_UPDATE_ICON;
            nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            Shell_NotifyIcon(NIM_ADD, &nid);

            // Обновляем иконку с начальной буквой
            UpdateIcon('A');
            break;

        case WM_APP_UPDATE_ICON:
            // Обновляем иконку в зависимости от переменной
            static char currentLetter = 'A';
            UpdateIcon(currentLetter);
            currentLetter = (currentLetter == 'A') ? 'B' : 'A';
            break;

        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Регистрируем класс окна
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TaskbarIconApp";
    RegisterClassEx(&wc);

    // Создаем окно
    hwnd = CreateWindowEx(0, "TaskbarIconApp", "Taskbar Icon App", 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Завершение работы GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

Описание кода:

    Инициализация иконки в панели задач:

        Используется структура NOTIFYICONDATA для добавления иконки в панель задач.

        Иконка создается с помощью GDI+ и отображает одну букву.

    Обновление иконки:

        Функция UpdateIcon создает новую иконку с буквой, которая передается в качестве аргумента.

        Иконка обновляется в панели задач с помощью Shell_NotifyIcon.

    Обработка сообщений:

        В цикле сообщений обрабатывается событие WM_APP_UPDATE_ICON, которое обновляет иконку.

    GDI+:

        Используется GDI+ для рисования буквы на иконке.

Как использовать:

    В этом примере иконка будет переключаться между буквами 'A' и 'B' при каждом вызове WM_APP_UPDATE_ICON.

    Вы можете изменить логику обновления иконки в зависимости от вашей переменной.

Компиляция:

    Убедитесь, что вы связали вашу программу с библиотекой gdiplus.lib.

Этот код является базовым примером, и его можно расширить для более сложных сценариев.
GetMessage не возращает управление при таком вызове.

Вы правы, проблема в том, что GetMessage не возвращает управление, если нет сообщений в очереди. В вашем случае, если нет сообщений, которые нужно обработать, GetMessage будет ждать, пока такое сообщение не появится. Это может быть проблемой, если вам нужно обновлять иконку в панели задач в зависимости от изменения переменной, но при этом нет других сообщений в очереди.

Чтобы решить эту проблему, можно использовать таймер для периодического обновления иконки. Таймер будет отправлять сообщение WM_TIMER в ваше окно через заданные интервалы времени, что позволит GetMessage возвращать управление и обновлять иконку.

Вот исправленный пример с использованием таймера:
cpp
Copy

#include <windows.h>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

#define WM_APP_UPDATE_ICON (WM_APP + 1)
#define IDT_TIMER 1

NOTIFYICONDATA nid = {};
HWND hwnd;
HICON hIcon = nullptr;

void UpdateIcon(char letter) {
    if (hIcon) {
        DestroyIcon(hIcon);
    }

    // Создаем временный контекст устройства
    HDC hdc = CreateCompatibleDC(nullptr);
    if (!hdc) return;

    // Создаем битмап для иконки
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 16, 16);
    if (!hBitmap) {
        DeleteDC(hdc);
        return;
    }

    SelectObject(hdc, hBitmap);

    // Очищаем фон
    RECT rect = {0, 0, 16, 16};
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // Рисуем букву
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, &letter, 1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Создаем иконку из битмапа
    ICONINFO iconInfo = {0};
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hBitmap;
    hIcon = CreateIconIndirect(&iconInfo);

    // Освобождаем ресурсы
    DeleteObject(hBitmap);
    DeleteDC(hdc);

    // Обновляем иконку в панели задач
    nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // Инициализируем структуру NOTIFYICONDATA
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_APP_UPDATE_ICON;
            nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            Shell_NotifyIcon(NIM_ADD, &nid);

            // Устанавливаем таймер для обновления иконки каждую секунду
            SetTimer(hwnd, IDT_TIMER, 1000, nullptr);
            break;

        case WM_TIMER:
            // Обновляем иконку в зависимости от переменной
            static char currentLetter = 'A';
            UpdateIcon(currentLetter);
            currentLetter = (currentLetter == 'A') ? 'B' : 'A';
            break;

        case WM_DESTROY:
            // Удаляем иконку из панели задач и завершаем работу
            Shell_NotifyIcon(NIM_DELETE, &nid);
            KillTimer(hwnd, IDT_TIMER);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Регистрируем класс окна
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TaskbarIconApp";
    RegisterClassEx(&wc);

    // Создаем окно
    hwnd = CreateWindowEx(0, "TaskbarIconApp", "Taskbar Icon App", 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Завершение работы GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

Что изменилось:

    Таймер:

        В WM_CREATE добавлен вызов SetTimer, который устанавливает таймер с интервалом 1000 мс (1 секунда).

        В обработчике WM_TIMER обновляется иконка.

    Обновление иконки:

        Теперь иконка обновляется каждую секунду, переключаясь между буквами 'A' и 'B'.

    Удаление таймера:

        В WM_DESTROY добавлен вызов KillTimer, чтобы удалить таймер при завершении программы.

Как это работает:

    Таймер периодически отправляет сообщение WM_TIMER в окно.

    В обработчике WM_TIMER вызывается функция UpdateIcon, которая обновляет иконку в панели задач.

    Это гарантирует, что GetMessage будет возвращать управление, даже если других сообщений в очереди нет.

Настройка интервала:

    Вы можете изменить интервал таймера, передав другое значение в SetTimer. Например, SetTimer(hwnd, IDT_TIMER, 500, nullptr) будет обновлять иконку каждые 500 мс.

Этот подход решает проблему с GetMessage и позволяет обновлять иконку в панели задач в зависимости от изменения переменной.
А можно, чтобы GetMessage возращал управление только тогда, когда в windows была нажата клавиша? Это уменьшило бы нагрузку на процессор.

Да, можно сделать так, чтобы GetMessage возвращал управление только при нажатии клавиши или других событиях, связанных с вводом. Это действительно уменьшит нагрузку на процессор, так как программа не будет активно опрашивать состояние системы (например, с помощью таймера).

Для этого можно использовать сообщения о вводе с клавиатуры (например, WM_KEYDOWN или WM_CHAR). Однако, если вы хотите, чтобы программа реагировала только на нажатия клавиш и больше ни на что, вам нужно будет явно игнорировать другие сообщения.

Вот пример, как это можно реализовать:
cpp
Copy

#include <windows.h>
#include <string>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

NOTIFYICONDATA nid = {};
HWND hwnd;
HICON hIcon = nullptr;

void UpdateIcon(char letter) {
    if (hIcon) {
        DestroyIcon(hIcon);
    }

    // Создаем временный контекст устройства
    HDC hdc = CreateCompatibleDC(nullptr);
    if (!hdc) return;

    // Создаем битмап для иконки
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 16, 16);
    if (!hBitmap) {
        DeleteDC(hdc);
        return;
    }

    SelectObject(hdc, hBitmap);

    // Очищаем фон
    RECT rect = {0, 0, 16, 16};
    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // Рисуем букву
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, &letter, 1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Создаем иконку из битмапа
    ICONINFO iconInfo = {0};
    iconInfo.fIcon = TRUE;
    iconInfo.hbmColor = hBitmap;
    iconInfo.hbmMask = hBitmap;
    hIcon = CreateIconIndirect(&iconInfo);

    // Освобождаем ресурсы
    DeleteObject(hBitmap);
    DeleteDC(hdc);

    // Обновляем иконку в панели задач
    nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            // Инициализируем структуру NOTIFYICONDATA
            nid.cbSize = sizeof(NOTIFYICONDATA);
            nid.hWnd = hwnd;
            nid.uID = 1;
            nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            nid.uCallbackMessage = WM_APP + 1;
            nid.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            Shell_NotifyIcon(NIM_ADD, &nid);

            // Устанавливаем начальную иконку
            UpdateIcon('A');
            break;

        case WM_KEYDOWN:
            // Обновляем иконку при нажатии клавиши
            static char currentLetter = 'A';
            UpdateIcon(currentLetter);
            currentLetter = (currentLetter == 'A') ? 'B' : 'A';
            break;

        case WM_DESTROY:
            // Удаляем иконку из панели задач и завершаем работу
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Регистрируем класс окна
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TaskbarIconApp";
    RegisterClassEx(&wc);

    // Создаем окно
    hwnd = CreateWindowEx(0, "TaskbarIconApp", "Taskbar Icon App", 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        // Обрабатываем только сообщения о нажатии клавиш
        if (msg.message == WM_KEYDOWN) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Завершение работы GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

Что изменилось:

    Обработка только WM_KEYDOWN:

        В цикле сообщений (while (GetMessage(...))) добавлена проверка на msg.message == WM_KEYDOWN. Это гарантирует, что программа будет реагировать только на нажатия клавиш.

        Остальные сообщения игнорируются.

    Обновление иконки при нажатии клавиши:

        В обработчике WM_KEYDOWN вызывается функция UpdateIcon, которая обновляет иконку в панели задач.

    Минимальная нагрузка на процессор:

        Поскольку программа реагирует только на нажатия клавиш, она не потребляет ресурсы процессора в фоновом режиме.

Как это работает:

    Программа создает окно и добавляет иконку в панель задач.

    В цикле сообщений программа ждет только сообщений о нажатии клавиш (WM_KEYDOWN).

    При нажатии клавиши иконка обновляется (в данном примере переключается между 'A' и 'B').

Преимущества:

    Минимальная нагрузка на процессор, так как программа не выполняет активного опроса.

    Простота реализации.

Недостатки:

    Программа не будет реагировать на другие события (например, клики мыши или сообщения от других приложений). Если это важно, можно расширить логику обработки сообщений.

Этот подход идеально подходит, если вам нужно, чтобы программа реагировала только на нажатия клавиш и больше ни на что.
Как приложение узнает о событии WM_KEYDOWN, если кнопка была нажата не в данном приложении, а в каких-то других приложениях?

The server is busy. Please try again later.
New chat
AI-generated, for reference only


#endif
