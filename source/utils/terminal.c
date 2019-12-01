#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "utils/terminal.h"
#include "utils/assert.h"

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

void terminal_set_cursor_position(int x, int y)
{
    printf("\e[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

void terminal_get_size(int *width, int *height)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *width = w.ws_col;
    *height = w.ws_row;
}

void terminal_clear(void)
{
    printf("\e[J");
    fflush(stdout);
}

int termianl_read_key(void)
{
    terminal_enter_rawmode();

    char c;
    assert(read(STDIN_FILENO, &c, 1) == 1);

    terminal_exit_rawmode();

    return c;
}

void terminal_save_cursor(void)
{
}

void terminal_restore_cursor(void)
{
}

void terminal_enable_alternative_screen_buffer(void)
{
    printf("\e[?1049h");
    fflush(stdout);
}

void terminal_disable_alternative_screen_buffer(void)
{
    printf("\e[?1049l");
    fflush(stdout);
}

void terminal_hide_cursor(void)
{
    printf("\e[?25l");
}

void terminal_show_cursor(void)
{
    printf("\033[?25h");
}