#ifdef LINUX
#include <stdio.h>
#include "conion.h"

#ifdef STD_CONSOLE_INPUT
void initialize_console_io()
{
}

void restore_console_io()
{
}

int kbhit(void)
{
    return 0;
}

char getch(void)
{
    return getchar();
}
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios oldt;
static int oldf;
static int lastkey = EOF;
static int initialized = 0;

void initialize_console_io()
{
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    initialized = 1;
}

void restore_console_io()
{
    if (initialized)
    {
        initialized = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    }
}

int kbhit(void)
{
    if (lastkey == EOF)
        lastkey = getchar();

    return (lastkey != EOF);
}

char getch(void)
{
    int ch;

    while (!kbhit());
    ch = lastkey;
    lastkey = EOF;
    return ch;
}
#endif
#endif
