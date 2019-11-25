#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "utils/string.h"
#include "utils/input.h"

void setup_terminal_for_user_input(void)
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
}

void restore_terminal_after_user_input(void)
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
}

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

int user_select(const char **options)
{
    char c;
    bool stop = false;
    int selected = 0;

    while (!stop)
    {
        printf("\e[1;1H\e[2J");

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

    return selected;
}

void user_input(const char *format, char *result, ListCallback list_callback, void *list_callback_args)
{
    (void)list_callback;
    (void)list_callback_args;
    char c;
    int index = 0;

    printf("\e[1;1H\e[2J");
    fflush(stdout);

    printf("\e[s");
    do
    {
        printf("%s%s\e[0m\n", user_input_valid(format, result) ? "\e[32m" : "\e[31m", result);

        printf("\e[J");

        if (list_callback)
        {
            list_callback(result, list_callback_args);
        }

        fflush(stdout);

        printf("\e[u");
        printf("\e[s");

        printf("\e[37m");

        printf("%s", format);

        printf("\e[0m");

        assert(read(STDIN_FILENO, &c, 1) == 1);

        printf("\e[u");

        printf("\e[s");

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

    } while (c != '\n');
}