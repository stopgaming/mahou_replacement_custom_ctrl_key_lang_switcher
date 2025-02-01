#include <iostream>
#include <windows.h>

bool isKeyPressed(int vkCode) {
    return (GetAsyncKeyState(vkCode) & 0x8000) != 0;
}

void isAnyOtherKeyPressed(int *anyother_cnt, int *lctrl_cnt, int *rctrl_cnt, int *nokeys_cnt) {
    // Проверяем все виртуальные коды клавиш (от 0x01 до 0xFE)
    int vkCode = 0x01;
    int pressed_smthgs = 0;
    for (vkCode = 0x01; vkCode <= 0xFE; vkCode++) {
        if (vkCode == VK_CONTROL)
            continue;
        if (isKeyPressed(vkCode)) {
            pressed_smthgs = 1;
            if (vkCode == VK_LCONTROL)
                (*lctrl_cnt)++;
            else if (vkCode == VK_RCONTROL)
                (*rctrl_cnt)++;
            else
                (*anyother_cnt)++;
        }
    }
    if (!pressed_smthgs)
        (*nokeys_cnt)++;
}

static int anyother_cnt = 0;
static int lctrl_cnt = 0;
static int rctrl_cnt = 0;
static int nokeys_cnt = 0;

static int state = 0;
static int state_key = 0;

void alt_switch(void) {
//    Альтернативный способ с использованием SendInput:
//    SendInput — это более современный и гибкий способ эмуляции ввода. Вот пример:
    INPUT inputs[4] = {};
    // Нажатие Alt
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_MENU;
    // Нажатие Shift
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_SHIFT;
    // Отпускание Shift
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_SHIFT;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    // Отпускание Alt
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_MENU;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    // Отправка всех событий
    SendInput(4, inputs, sizeof(INPUT));
}

void alt_switch_ctrl9(void) {
//    Альтернативный способ с использованием SendInput:
//    SendInput — это более современный и гибкий способ эмуляции ввода. Вот пример:
    INPUT inputs[4] = {};
    // Нажатие VK_CONTROL
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    // Нажатие 9
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0x39; //  	9 клавиш
    // Отпускание 9
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 0x39;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    // Отпускание VK_CONTROL
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    // Отправка всех событий
    SendInput(4, inputs, sizeof(INPUT));
}
void alt_switch_ctrl0(void) {
//    Альтернативный способ с использованием SendInput:
//    SendInput — это более современный и гибкий способ эмуляции ввода. Вот пример:
    INPUT inputs[4] = {};
    // Нажатие VK_CONTROL
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    // Нажатие 0
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0x30; //  	0 клавиш
    // Отпускание 0
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 0x30;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    // Отпускание VK_CONTROL
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    // Отправка всех событий
    SendInput(4, inputs, sizeof(INPUT));
}

int state_key_after_send=0;
static void event1_cb(int state_key) {
    //alt_switch();
    if (state_key == 1) {
        alt_switch_ctrl9();
        state_key_after_send = state_key;
    }
    else
        if (state_key == 2)
        {
            alt_switch_ctrl0();
            state_key_after_send = state_key;
        }
}

void main_step(void) {
    {
        // Проверяем, нажата ли любая другая клавиша
        isAnyOtherKeyPressed(&anyother_cnt, &lctrl_cnt, &rctrl_cnt, &nokeys_cnt);

        if (nokeys_cnt) {
            if (state == 2) {
                event1_cb(state_key);
                // std::cout << "state_key " << state_key << std::endl;
            }
            state = 1;
        } else if (state == 1) {
            if (lctrl_cnt || rctrl_cnt) {
                state = 2;
                if (lctrl_cnt > rctrl_cnt)
                    state_key = 1;
                else
                    state_key = 2;
            } else
                state = 0;
        }
        if (anyother_cnt) {
            state = 0;
            //std::cout << "state " << state << std::endl;
        }


        anyother_cnt = 0;
        lctrl_cnt = 0;
        rctrl_cnt = 0;
        nokeys_cnt = 0;

        Sleep(1); // Небольшая задержка для снижения нагрузки на процессор
    }
}

// https://learn.microsoft.com/ru-ru/windows/win32/inputdev/virtual-key-codes
