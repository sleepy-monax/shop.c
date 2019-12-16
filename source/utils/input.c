#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "model/view.h"
#include "utils/input.h"
#include "utils/renderer.h"
#include "utils/string.h"
#include "utils/terminal.h"

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

bool user_yes_no(const char *prompt, bool default_choice)
{

    if (default_choice == NO)
    {
        printf("%s [Oui/\e[1mNon\e[0m]", prompt);
    }
    else
    {
        printf("%s [\e[1mOui\e[0m/Non]", prompt);
    }

    char c = terminal_read_key();

    if (default_choice == NO)
    {

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
    else
    {
        if (c == 'N' || c == 'n')
        {
            printf(" Non\n");

            return false;
        }
        else
        {
            printf(" Oui\n");

            return true;
        }
    }
}

int user_select(User *user, const char *prompt, const char *options[])
{
    terminal_enable_alternative_screen_buffer();

    Surface *surface = surface_create();

    bool stop = false;
    int selected = 0;

    while (!stop)
    {
        surface_clear(surface, DEFAULT_STYLE);
        model_view_title(user, surface, prompt);

        for (int i = 0; options[i]; i++)
        {
            if (i == selected)
            {
                surface_text(surface, vstringf("> %s <", options[i]).as_string, 0, i, surface_width(surface), style_centered(style_bold(DEFAULT_STYLE)));
            }
            else
            {
                surface_text(surface, options[i], 0, i, surface_width(surface), style_centered(DEFAULT_STYLE));
            }
        }

        surface_text(surface, " [K] up / [J] down / [ENTER] select", 0, surface_height(surface) - 1, surface_width(surface), style_inverted(DEFAULT_STYLE));

        surface_pop_clip(surface);

        surface_render(surface);
        surface_update(surface);

        char c = terminal_read_key();

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

    surface_destroy(surface);

    terminal_disable_alternative_screen_buffer();

    return selected;
}

void user_input_password(const char *prompt, char *result, int n)
{
    terminal_enter_rawmode();

    char c;
    int index = 0;

    result[0] = '\0';

    printf("%s: ", prompt);

    do
    {
        c = terminal_read_key();

        if (c == 127 && index > 0)
        {
            index--;
            result[index] = '\0';
        }
        else if (iscntrl(c))
        {
            // do nothing with it
        }
        else if (index < n)
        {
            result[index] = c;
            result[index + 1] = '\0';
            index++;
        }
    } while (c != '\n');

    printf("\n");

    terminal_exit_rawmode();
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

        c = terminal_read_key();

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