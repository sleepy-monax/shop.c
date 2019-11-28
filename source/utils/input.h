#pragma once

#include <stdbool.h>

typedef enum
{
    INPUT_INVALID,
    INPUT_VALID,
    INPUT_OK,
} InputValidState;

typedef void (*ListCallback)(const char *user_input, void *args);

void setup_terminal_for_user_input(void);

void restore_terminal_after_user_input(void);

bool user_yes_no(const char *prompt);

int user_select(const char *prompt, const char **options);

void user_input(const char *prompt, const char *format, char *result);
