#include <stdio.h>
#include <string.h>

#include "utils/assert.h"
#include "model/model_view.h"
#include "utils/terminal.h"
#include "utils/math.h"

void model_draw_title(const char *title, int width)
{
    int len = strlen(title);
    int padding = (width - len) / 2;

    printf("\e[1m");

    for (int i = 0; i < padding; i++)
    {
        printf(" ");
    }

    printf("%s", title);

    for (int i = 0; i < padding; i++)
    {
        printf(" ");
    }

    printf("\n");

    for (int i = 0; i < width; i++)
    {
        printf("▔");
    }

    printf("\e[0m");
}

void model_view_draw_header(ModelViewState state, Model model, int column)
{
    int column_width = state.width / model.column_count();
    int padding = column_width - strlen(model.column_name(column));

    for (int i = 0; i < padding / 2; i++)
    {
        printf(" ");
    }

    printf("%s", model.column_name(column));

    for (int i = (padding / 2) + strlen(model.column_name(column)); i < column_width; i++)
    {
        printf(" ");
    }
}

void model_view_draw_cell(ModelViewState state, Model model, void *data, int row, int column)
{

    if (row == state.slected)
    {
        printf("\e[30;47m");
    }
    else if (row % 2)
    {
        printf("\e[37m");
    }
    else
    {
        printf("\e[90m");
    }

    int column_width = state.width / model.column_count();

    Variant value = model.get_data(data, row, column);

    if ((int)strlen(value.as_string) >= column_width)
    {
        value.as_string[max(0, column_width - 3)] = '\0';
        printf(" %s", value.as_string);
        printf("… ");
    }
    else
    {
        int padding = column_width - strlen(value.as_string) - 1;

        if (value.type == VARIANT_STRING)
        {
            printf(" %s", value.as_string);

            for (int i = 0; i < padding; i++)
            {
                printf(" ");
            }
        }
        else
        {
            for (int i = 0; i < padding; i++)
            {
                printf(" ");
            }

            printf("%s ", value.as_string);
        }
    }

    printf("\e[0m");
}

void model_view_display(const char *title, ModelViewState state, Model model, void *data)
{
    (void)state;

    terminal_set_cursor_position(0, 0);

    model_draw_title(title, state.width);

    printf("\n");
    for (int i = 0; i < model.column_count(); i++)
    {
        model_view_draw_header(state, model, i);
    }

    for (int i = 0; i < state.width % model.column_count(); i++)
    {
        printf(" ");
    }

    printf("\n");

    for (int i = 0; i < state.width; i++)
    {
        printf("-");
    }

    printf("\n");

    for (int row = state.scroll; row < model.row_count(data); row++)
    {
        for (int column = 0; column < model.column_count(); column++)
        {
            model_view_draw_cell(state, model, data, row, column);
        }

        for (int i = 0; i < state.width % model.column_count(); i++)
        {
            printf(" ");
        }

        printf("\n");
    }

    terminal_clear();
}

void model_view_edit(ModelViewState state, Model model, void *data, int row)
{
    terminal_set_cursor_position(0, 0);
    terminal_clear();

    model_draw_title("Editer une valeur", state.width);

    for (int i = 0; i < model.column_count(); i++)
    {
        printf("\e[1m%16s\e[0m: %s\n", model.column_name(i), model.get_data(data, row, i).as_string);
    }

    termianl_read_key();
}

void model_view(const char *title, Model model, void *data)
{
    ModelViewState state = {0};

    terminal_enable_alternative_screen_buffer();
    terminal_hide_cursor();

    do
    {
        terminal_get_size(&state.width, &state.height);
        model_view_display(title, state, model, data);

        int key = termianl_read_key();

        if (key == 'q')
            state.exited = true;

        if (key == 'j')
            state.slected = min(model.row_count(data) - 1, state.slected + 1);

        if (key == 'k')
            state.slected = max(0, state.slected - 1);

        if (key == 'J')
            state.slected = min(model.row_count(data) - 1, state.slected + 10);

        if (key == 'K')
            state.slected = max(0, state.slected - 10);

        if (key == 'e')
            model_view_edit(state, model, data, state.slected);

    } while (!state.exited);

    terminal_show_cursor();
    terminal_disable_alternative_screen_buffer();
}
