#pragma once

typedef enum
{
    INPUT_INVALID,
    INPUT_VALID,
    INPUT_OK,
} InputValidState;

typedef void (*ListCallback)(const char *user_input, void *args);

void setup_terminal_for_user_input(void);

void restore_terminal_after_user_input(void);

int user_select(const char **options);

void user_input(const char *format, char *result, ListCallback list_callback, void *list_callback_args);
