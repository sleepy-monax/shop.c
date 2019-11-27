#pragma once

typedef struct
{
    int X;
    int Y;
} TerminalPosition;

void terminal_enter_rawmode(void);

void terminal_exit_rawmode(void);

void terminal_set_cursor_position(TerminalPosition position);

void terminal_clear(void);

void terminal_save_cursor(void);

void terminal_restore_cursor(void);