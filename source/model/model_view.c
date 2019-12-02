#include <stdio.h>
#include <string.h>

#include "utils/string.h"
#include "utils/assert.h"
#include "model/model_view.h"
#include "utils/terminal.h"
#include "utils/math.h"

void model_view_draw_title(const char *title, int width)
{
    terminal_set_cursor_position(0, 0);

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
    int padding = column_width - strlen_unicode(model.column_name(column, ROLE_DISPLAY)) - 2;

    for (int i = 0; i < padding / 2; i++)
    {
        printf(" ");
    }

    if (state.sortby == column)
    {
        if (state.sort_accending)
        {
            printf("\e[1m⌃ %s\e[0m", model.column_name(column, ROLE_DISPLAY));
        }
        else
        {
            printf("\e[1m⌄ %s\e[0m", model.column_name(column, ROLE_DISPLAY));
        }
    }
    else
    {
        printf("  %s", model.column_name(column, ROLE_DISPLAY));
    }

    for (int i = (padding / 2) + strlen_unicode(model.column_name(column, ROLE_DISPLAY)) + 2; i < column_width; i++)
    {
        printf(" ");
    }
}

void model_view_draw_cell(ModelViewState state, Model model, void *data, int row, int column)
{
    int column_width = state.width / model.column_count();

    Variant value = model.get_data(data, row, column, ROLE_DISPLAY);

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

void model_view_draw_status_bar(ModelViewState state, Model model, void *data, const char *msg)
{
    terminal_set_cursor_position(0, state.height - 1);
    terminal_clear();

    printf("\e[30;44m 👤 Manager \e[0m");

    if (msg)
    {
        printf(" %s ", msg);
    }
    else
    {
        printf(" 💡 Appuyer sur 'h' pour afficher l'aide - %d éléments - ligne %d ", model.row_count(data), state.slected + 1);
    }

    printf("\e[0m");
}

void model_view_scrollbar(ModelViewState state, Model model, void *data)
{
    if (model.row_count(data) > state.height - 5)
    {
        float viewport_height = state.height - 5;
        float content_height = model.row_count(data);

        float viewable_ratio = viewport_height / content_height;
        float scroll_bar_area = viewport_height;
        float thump_pos = (state.scroll / (float)model.row_count(data)) * viewport_height;
        float thumb_height = scroll_bar_area * viewable_ratio;

        for (int i = 4; i < state.height - 1; i++)
        {
            terminal_set_cursor_position(state.width - 1, i);
            printf(" ");
        }

        for (int i = 0; i < thumb_height; i++)
        {
            terminal_set_cursor_position(state.width - 1, thump_pos + 4 + i);
            printf("▐");
        }
    }
}

void model_view_display(const char *title, ModelViewState state, Model model, void *data)
{
    (void)state;

    terminal_set_cursor_position(0, 0);

    model_view_draw_title(title, state.width);

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
    model_view_scrollbar(state, model, data);
    model_view_draw_status_bar(state, model, data, NULL);
}

void model_view_edit(ModelViewState state, Model model, void *data, int row)
{
    terminal_set_cursor_position(0, 0);
    terminal_clear();

    model_view_draw_title("Éditer une valeur", state.width);

    for (int i = 0; i < model.column_count(); i++)
    {
        printf("\e[1m%16s\e[0m: %s\n", model.column_name(i, ROLE_DISPLAY), model.get_data(data, row, i, ROLE_DISPLAY).as_string);
    }

    termianl_read_key();
}

void reverse_array(int a[], int n)
{
    int c, t;

    for (c = 0; c < n / 2; c++)
    {
        t = a[c];
        a[c] = a[n - c - 1];
        a[n - c - 1] = t;
    }
}

void model_view(const char *title, Model model, void *data)
{
    ModelViewState state = {0};
    state.sort_dirty = true;

    terminal_enable_alternative_screen_buffer();
    terminal_hide_cursor();
    terminal_enter_rawmode();

    do
    {
        terminal_get_size(&state.width, &state.height);

        if (state.sort_dirty)
        {
            for (int i = 0; i < model.row_count(data); i++)
            {
                state.sorted[i] = i;
            }

            for (int i = 0; i < model.row_count(data) - 1; i++)
            {
                Variant idata = model.get_data(data, state.sorted[i], state.sortby, ROLE_DATA);

                for (int j = i + 1; j < model.row_count(data); j++)
                {
                    Variant jdata = model.get_data(data, state.sorted[j], state.sortby, ROLE_DATA);

                    int cmp = variant_cmp(idata, jdata);

                    if ((cmp > 0 && !state.sort_accending) || (cmp < 0 && state.sort_accending))
                    {
                        int tmp = state.sorted[i];
                        state.sorted[i] = state.sorted[j];
                        state.sorted[j] = tmp;

                        idata = jdata;
                    }
                }
            }

            state.sort_dirty = false;
        }

        model_view_display(title, state, model, data);

        int codepoint = termianl_read_key();

        for (int i = 0; model.get_actions()[i].key_codepoint != 0; i++)
        {
            ModelAction action = model.get_actions()[i];

            if (action.key_codepoint == codepoint)
            {
                action.callback(codepoint, &state, model, data, state.sorted[state.slected]);
            }
        }

        if (codepoint > '0' && codepoint <= '9')
        {
            int new_sort_by = codepoint - '0' - 1;

            if (new_sort_by < model.column_count())
            {
                if (state.sortby == new_sort_by)
                {
                    state.sort_accending = !state.sort_accending;
                    reverse_array(state.sorted, model.row_count(data));
                }
                else
                {
                    state.sortby = new_sort_by;
                    state.sort_accending = false;

                    state.sort_dirty = true;
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

    } while (!state.exited);

    terminal_exit_rawmode();
    terminal_show_cursor();
    terminal_disable_alternative_screen_buffer();
}
