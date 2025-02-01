cmake_minimum_required(VERSION 3.30)
project(keyswitcher)

set(CMAKE_CXX_STANDARD 98)

add_executable(keyswitcher WIN32 main.cpp
        ico_app_task_main.cpp)
