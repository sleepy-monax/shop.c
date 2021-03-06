#pragma once

#include <stdbool.h>

#include "model/user.h"

typedef enum
{
    INPUT_INVALID,
    INPUT_VALID,
    INPUT_OK,
} InputValidState;

typedef void (*ListCallback)(const char *user_input, void *args);

void setup_terminal_for_user_input(void);

void restore_terminal_after_user_input(void);

#define YES 1
#define NO 0

bool user_yes_no(const char *prompt, bool default_choice);

int user_select(User *user, const char *prompt, const char *options[]);

void user_input(const char *prompt, const char *format, char *result);

void user_input_password(const char *prompt, char *result, int n);
