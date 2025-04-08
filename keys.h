#ifndef ___WYLAND_BUILT_IN_RUNTIME_KEYS___
#define ___WYLAND_BUILT_IN_RUNTIME_KEYS___

#include <stdio.h>
#include <stdint.h>

#include "wylrt.h"

#if defined(_WIN32) || defined(_WIN64) // Windows
    #include <conio.h>
    #include <windows.h>

    wyland_uint get_key() {
        if (_kbhit()) {
            wyland_uint key = (wyland_uint)_getch();

            if (key == 0 || key == 224) {
                key = (wyland_uint)_getch();
                return 256 + key;
            }

            return key;
        }
        return 0;
    }

#else // Linux/macOS
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

    void enable_raw_mode() {
        struct termios raw;
        tcgetattr(STDIN_FILENO, &raw);
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    void disable_raw_mode() {
        struct termios raw;
        tcgetattr(STDIN_FILENO, &raw);
        raw.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    wyland_uint get_key() {
        struct termios oldt, newt;
        wyland_uint ch;
        int oldf;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = (wyland_uint)getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        return (ch != EOF) ? ch : 0;
    }
#endif

#endif // ___WYLAND_BUILT_IN_RUNTIME_KEYS___