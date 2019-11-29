#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "utils/string.h"
#include "utils/input.h"
#include "utils/terminal.h"

//void user_input_display(const char *format, const char *input)
//{
//    int format_index = 0;
//    int input_index = 0;
//}

InputValidState user_input_valid(const char *format, const char *input)
{
    for (int i = 0; input[i]; i++)
    {
        char in_c = input[i];
        if (format[i] == '_' && !(isalpha(in_c) || isdigit(in_c)))
            return INPUT_INVALID;

        if (format[i] == '.' && !isalpha(in_c))
            return INPUT_INVALID;

        if (format[i] == '#' && !isdigit(in_c))
            return INPUT_INVALID;
    }

    return INPUT_VALID;
}

bool user_yes_no(const char *prompt)
{
    terminal_enter_rawmode();

    printf("%s [Oui/Non]", prompt);
    fflush(stdout);

    char c;
    assert(read(STDIN_FILENO, &c, 1) == 1);

    terminal_exit_rawmode();

    if (c == 'Y' || c == 'O' || c == 'y' || c == 'o')
    {
        printf(" Oui\n");

        return true;
    }
    else
    {
        printf(" Non\n");

        return false;
    }
}

int user_select(const char *prompt, const char *options[])
{
    terminal_enter_rawmode();
    terminal_enable_alternative_screen_buffer();

    char c;
    bool stop = false;
    int selected = 0;

    while (!stop)
    {
        printf("\e[1;1H\e[2J");

        printf("%s:\n\n", prompt);

        for (int i = 0; options[i]; i++)
        {
            if (i == selected)
            {
                printf("\e[32;1m-> \e[0m%s\n", options[i]);
            }
            else
            {
                printf("   %s\n", options[i]);
            }
        }

        printf("\n[K] up / [J] down / [ENTER] select\n");

        assert(read(STDIN_FILENO, &c, 1) == 1);

        if (c == 'k')
        {
            if (selected > 0)
                selected--;
        }
        else if (c == 'j')
        {
            if (options[selected + 1] != NULL)
                selected++;
        }
        else if (c == '\n')
        {
            stop = true;
        }
    }

    terminal_disable_alternative_screen_buffer();
    terminal_exit_rawmode();

    return selected;
}

void user_input(const char *prompt, const char *format, char *result)
{
    terminal_enter_rawmode();

    char c;
    int index = 0;

    result[0] = '\0';

    printf("\e[?25l%s: ", prompt);

    do
    {
        printf("\e[s\e[37m%s\e[0m\e[u", format);
        printf("\e[s%s%s\e[0m\e[u", user_input_valid(format, result) ? "\e[32m" : "\e[31m", result);
        fflush(stdout);

        assert(read(STDIN_FILENO, &c, 1) == 1);

        if (c == 127 && index > 0)
        {
            index--;
            result[index] = '\0';
        }
        else if (iscntrl(c))
        {
            // do nothing with it
        }
        else if (index < (int)strlen(format))
        {
            result[index] = c;
            result[index + 1] = '\0';
            index++;
        }
    } while (c != '\n' || !user_input_valid(format, result));

    printf("\033[?25h\n");

    terminal_exit_rawmode();
}