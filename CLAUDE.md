# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

**Minimum requirement**: CMake 3.30+

### Static build with MinGW
```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=path/to/mingw.cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Development build (dynamic linking)
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Manual static build with MinGW
```bash
g++ -static -static-libgcc -static-libstdc++ -mwindows -o keyswitcher.exe main.cpp ico_app_task_main.cpp -lgdiplus
```

## Architecture

**Application**: A Windows taskbar icon application that monitors Control key presses to trigger keyboard layout switching via `Ctrl-Shift-9` (English) and `Ctrl-Shift-0` (Russian).

### Core Components

**main.cpp** - Layout switching logic:
- `main_step()`: Polling loop checking for Control key combinations
- `isAnyOtherKeyPressed()`: Detects pressed keys (0x01-0xFE virtual codes)
- `alt_switch_ctrl9()` / `alt_switch_ctrl0()`: Simulates `Ctrl-Shift-[9/0]` via `SendInput()`
- State machine: `state=0` → `state=1` (no keys) → `state=2` (Control detected) → trigger switch

**ico_app_task_main.cpp** - GUI and icon display:
- `MyThreadFunction()`: Separate thread that calls `main_step()`
- `WndProc()`: Window procedure handling taskbar icon creation and updates
- `UpdateIcon()`: Draws single-letter icon (E=English, R=Russian) using GDI+
- Runs GDI+ initialization once in `WinMain()`

### Key Windows API Usage

- `GetAsyncKeyState()`: Polls key state (short-lived polling approach)
- `SendInput()`: Emulates keyboard input to trigger Windows layout switching
- `Shell_NotifyIcon()`: Taskbar tray icon with NIF_MESSAGE callback
- GDI+: Runtime initialization via `GdiplusStartup()`/`GdiplusShutdown()`

### Interaction Flow

1. User presses Left Control → `state` transitions to 2 → `state_key=1` → `event1_cb()` sends `Ctrl-Shift-9`
2. User presses Right Control → `state` transitions to 2 → `state_key=2` → `event1_cb()` sends `Ctrl-Shift-0`
3. Pressing any other key resets `state` to 0
4. Tracked via taskbar icon showing 'E' or 'R'
