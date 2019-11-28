#include <termios.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils/terminal.h"

void terminal_enter_rawmode(void)
{
    struct termios info;
    int result = 0;

    result = tcgetattr(0, &info);

    if (result == -1)
    {
        perror("Failled to setup user input");
        abort();
    }

    info.c_lflag &= ~(ECHO | ICANON);

    result = tcsetattr(0, TCSANOW, &info);

    if (result == -1)
    {
        perror("Failled to setup user input");
        abort();
    }
}

void terminal_exit_rawmode(void)
{
    struct termios info;
    int result = 0;

    result = tcgetattr(0, &info);

    if (result == -1)
    {
        perror("Failled to setup user input");
        abort();
    }

    info.c_lflag |= (ECHO | ICANON);

    result = tcsetattr(0, TCSAFLUSH, &info);

    if (result == -1)
    {
        perror("Failled to setup user input");
        abort();
    }
}

void terminal_set_cursor_position(TerminalPosition position)
{
    printf("\e[%d:%dH", position.X + 1, position.Y + 1);
}

void terminal_clear(void)
{
}

void terminal_save_cursor(void)
{
}

void terminal_restore_cursor(void)
{
}
