#include <stdio.h>
#include <string.h>

#include "utils/string.h"
#include "utils/assert.h"
#include "model/model_view.h"
#include "utils/terminal.h"
#include "utils/math.h"

void model_draw_title(const char *title, int width)
{
    int len = strlen_unicode(title);
    int padding = (width - len) / 2;

    printf("\e[1m");

    for (int i = 0; i < padding; i++)
    {
        printf(" ");
    }

    printf("%s", title);

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
    int padding = column_width - strlen_unicode(model.column_name(column)) - 2;

    for (int i = 0; i < padding / 2; i++)
    {
        printf(" ");
    }

    if (state.sortby == column)
    {
        if (state.sort_accending)
        {
            printf("\e[1m⌃ %s\e[0m", model.column_name(column));
        }
        else
        {
            printf("\e[1m⌄ %s\e[0m", model.column_name(column));
        }
    }
    else
    {
        printf("  %s", model.column_name(column));
    }

    for (int i = (padding / 2) + strlen_unicode(model.column_name(column)) + 2; i < column_width; i++)
    {
        printf(" ");
    }
}

void model_view_draw_cell(ModelViewState state, Model model, void *data, int row, int column)
{
    int column_width = state.width / model.column_count();

    Variant value = model.get_data(data, row, column);

    if ((int)strlen_unicode(value.as_string) >= column_width)
    {
        value.as_string[max(0, column_width - 3)] = '\0';
        printf(" %s", value.as_string);
        printf("… ");
    }
    else
    {
        int padding = column_width - strlen_unicode(value.as_string) - 1;

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

    for (int row = state.scroll; row < min(state.scroll + state.height - 5, model.row_count(data)); row++)
    {
        if (row == state.slected)
        {
            printf("\e[30;47m");
        }
        else if (row % 2)
        {
            printf("\e[1m");
        }
        else
        {
            printf("\e[0m");
        }

        for (int column = 0; column < model.column_count(); column++)
        {
            model_view_draw_cell(state, model, data, state.sorted[row], column);
        }

        for (int i = 0; i < state.width % model.column_count(); i++)
        {
            printf(" ");
        }

        printf("\e[0m");
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
    terminal_enter_rawmode();

    do
    {
        for (int i = 0; i < model.row_count(data); i++)
        {
            state.sorted[i] = i;
        }

        for (int i = 0; i < model.row_count(data) - 1; i++)
        {
            for (int j = i + 1; j < model.row_count(data); j++)
            {
                int cmp = strcmp(model.get_data(data, state.sorted[i], state.sortby).as_string,
                                 model.get_data(data, state.sorted[j], state.sortby).as_string);

                if ((cmp > 0 && !state.sort_accending) || (cmp < 0 && state.sort_accending))
                {
                    int tmp = state.sorted[i];

                    state.sorted[i] = state.sorted[j];
                    state.sorted[j] = tmp;
                }
            }
        }

        terminal_get_size(&state.width, &state.height);
        model_view_display(title, state, model, data);

        int key = termianl_read_key();

        if (key == 'q')
            state.exited = true;

        if (key == 'j')
            state.slected++;

        if (key == 'k')
            state.slected--;

        if (key == 'J')
            state.slected += 10;

        if (key == 'K')
            state.slected -= 10;

        if (key == 'e')
            model_view_edit(state, model, data, state.sorted[state.slected]);

        if (key == 'i')
            model_view_edit(state, model, data, model.row_create(data));

        if (key == 'd')
            model.row_delete(data, state.sorted[state.slected]);

        if (key > '0' && key <= '9')
        {
            int new_sort_by = key - '0' - 1;

            if (new_sort_by < model.column_count())
            {
                if (state.sortby == new_sort_by)
                {
                    state.sort_accending = !state.sort_accending;
                }
                else
                {
                    state.sortby = new_sort_by;
                    state.sort_accending = false;
                }
            }
        }

        state.slected = min(model.row_count(data) - 1, max(0, state.slected));

        if (state.slected < state.scroll)
        {
            state.scroll = state.slected;
        }

        if (state.slected >= state.scroll + (state.height - 5))
        {
            state.scroll = state.slected - (state.height - 5) + 1;
        }

        state.scroll = max(0, min(state.scroll, model.row_count(data) - 1));

        //state.scroll = max(state.slected, min(state.scroll, model.row_count(data) - 1 - state.height - 5));
    } while (!state.exited);

    terminal_exit_rawmode();
    terminal_show_cursor();
    terminal_disable_alternative_screen_buffer();
}
