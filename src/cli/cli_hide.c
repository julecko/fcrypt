#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#ifdef _WIN32
static DWORD originalMode;
static HANDLE hStdin = NULL;

void hide_input(void) {
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &originalMode);
    SetConsoleMode(hStdin, originalMode & ~(ENABLE_ECHO_INPUT));
}

void reveal_input(void) {
    if (hStdin) {
        SetConsoleMode(hStdin, originalMode);
        hStdin = NULL;
    }
}
#else
static struct termios oldt;
static bool hidden_active = false;

void hide_input(void) {
    tcgetattr(STDIN_FILENO, &oldt);
    struct termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    hidden_active = true;
}

void reveal_input(void) {
    if (hidden_active) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        hidden_active = false;
    }
}
#endif