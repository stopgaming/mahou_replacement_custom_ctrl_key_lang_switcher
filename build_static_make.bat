@echo off
REM Manual static build with MinGW (no CMake)
echo Building keyswitcher statically...
g++ -static -static-libgcc -static-libstdc++ -mwindows -O2 -o keyswitcher.exe main.cpp ico_app_task_main.cpp -lgdiplus

echo Done! Output: keyswitcher.exe
