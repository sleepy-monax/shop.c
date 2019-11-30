#pragma once

int termianl_read_key(void);

void terminal_enter_rawmode(void);

void terminal_exit_rawmode(void);

void terminal_set_cursor_position(int x, int y);

void terminal_get_size(int *width, int *height);

void terminal_clear(void);

void terminal_save_cursor(void);

void terminal_restore_cursor(void);

void terminal_enable_alternative_screen_buffer(void);

void terminal_disable_alternative_screen_buffer(void);

void terminal_hide_cursor(void);

void terminal_show_cursor(void);