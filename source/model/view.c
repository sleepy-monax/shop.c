#include "model/view.h"
#include "utils/math.h"
#include "utils/renderer.h"
#include "utils/terminal.h"

void model_view_title(User *user, Surface *surface, const char *title)
{
    if (user)
    {
        switch (user->access)
        {
        case ACCESS_ADMIN:
            surface_text(surface, title, 0, 1, surface_width(surface), style_bold(style_centered(RED_STYLE)));
            surface_text(surface, "ADMIN", 2, 2, 16, style_inverted(style_centered(RED_STYLE)));
            surface_plot_line(surface, u'▔', 0, 3, surface_width(surface), 3, RED_STYLE);

            break;

        case ACCESS_MANAGER:
            surface_text(surface, title, 0, 1, surface_width(surface), style_bold(style_centered(BLUE_STYLE)));
            surface_text(surface, "MANAGER", 2, 2, 16, style_inverted(style_centered(BLUE_STYLE)));
            surface_plot_line(surface, u'▔', 0, 3, surface_width(surface), 3, BLUE_STYLE);

            break;

        case ACCESS_CASHIER:
            surface_text(surface, title, 0, 1, surface_width(surface), style_bold(style_centered(WHITE_STYLE)));
            surface_text(surface, "CAISSIER", 2, 2, 16, style_inverted(style_centered(WHITE_STYLE)));
            surface_plot_line(surface, u'▔', 0, 3, surface_width(surface), 3, WHITE_STYLE);

            break;

        default:
            break;
        }

        char buffer[128];
        snprintf(buffer, 128, "%s %s", user->lastname, user->firstname);
        surface_text(surface, buffer, 20, 2, 36, DEFAULT_STYLE);
    }
    else
    {
        surface_text(surface, title, 0, 1, surface_width(surface), style_bold(style_centered(WHITE_STYLE)));
        surface_plot_line(surface, u'▔', 0, 3, surface_width(surface), 3, WHITE_STYLE);
    }

    surface_push_clip(surface, (Region){
                                   0,
                                   4,
                                   surface_width(surface),
                                   surface_height(surface) - 4,
                               });
}

void model_view_scrollbar(Surface *surface, ModelViewState *state, Model model, void *data)
{
    if (model.row_count(data) > surface_height(surface))
    {
        float viewport_height = surface_height(surface);
        float content_height = model.row_count(data);

        float viewable_ratio = viewport_height / content_height;
        float scroll_bar_area = viewport_height;
        float thump_pos = (state->scroll / (float)model.row_count(data)) * viewport_height;
        float thumb_height = scroll_bar_area * viewable_ratio;

        for (int i = 0; i < surface_height(surface); i++)
        {
            surface_plot(surface, ' ', surface_width(surface) - 1, i, DEFAULT_STYLE);
        }

        for (int i = 0; i < thumb_height; i++)
        {
            surface_plot(surface, u'▐', surface_width(surface) - 1, thump_pos + i, BLUE_STYLE);
        }
    }

    surface_push_clip(surface, (Region){0, 0, surface_width(surface) - 1, surface_height(surface)});
}

void model_view_headerbar(Surface *surface, ModelViewState *state, Model model)
{
    int column_width = surface_width(surface) / model.column_count();

    for (int column = 0; column < model.column_count(); column++)
    {
        if (state->sortby == column)
        {
            surface_text(surface, model.column_name(column, ROLE_DISPLAY), column * column_width, 0, column_width, style_centered(BOLD_STYLE));

            surface_plot(surface, state->sort_accending ? u'⌄' : u'⌃', column * column_width + 1, 0, DEFAULT_STYLE);
        }
        else
        {
            surface_text(surface, model.column_name(column, ROLE_DISPLAY), column * column_width, 0, column_width, style_centered(DEFAULT_STYLE));
        }
    }

    surface_plot_line(surface, u'-', 0, 1, surface_width(surface), 1, DEFAULT_STYLE);
    surface_push_clip(surface, (Region){0, 2, surface_width(surface), surface_height(surface) - 2});
}

void model_view_status_bar(Surface *surface, ModelViewState *state, Model model, void *data)
{
    char buffer[128];
    snprintf(buffer, 128, " %d éléments - ligne %d - [?] Appuyer sur 'h' pour afficher l'aide", model.row_count(data), state->slected + 1);
    surface_text(surface, buffer, 0, surface_height(surface) - 1, surface_width(surface), DEFAULT_STYLE);

    surface_push_clip(surface, (Region){0, 0, surface_width(surface), surface_height(surface) - 1});
}

void model_view_update_scroll(Surface *surface, ModelViewState *state, Model model, void *data)
{
    state->slected = min(model.row_count(data) - 1, max(0, state->slected));

    if (state->slected < state->scroll)
    {
        state->scroll = state->slected;
    }

    if (state->slected >= state->scroll + surface_height(surface))
    {
        state->scroll = state->slected - surface_height(surface) + 1;
    }

    state->scroll = max(0, min(state->scroll, model.row_count(data) - 1));
}

void model_view_list(User *user, Surface *surface, ModelViewState *state, Model model, void *data)
{
    model_view_scrollbar(surface, state, model, data);

    model_view_headerbar(surface, state, model);

    model_view_update_scroll(surface, state, model, data);

    int column_width = surface_width(surface) / model.column_count();

    for (int row = state->scroll; row < min(state->scroll + surface_height(surface), model.row_count(data)); row++)
    {
        for (int column = 0; column < model.column_count(); column++)
        {
            Variant value = model_get_data_with_access(model, data, state->sorted[row], column, user, ROLE_DISPLAY);

            if (row == state->slected)
            {
                surface_text(surface, value.as_string, column * column_width, row - state->scroll, column_width, style_with_foreground(style_with_background(model.column_style(column), COLOR_WHITE), COLOR_BLACK));
            }
            else
            {
                surface_text(surface, value.as_string, column * column_width, row - state->scroll, column_width, (row % 2) ? style_with_background(model.column_style(column), COLOR_BRIGHT_BLACK) : model.column_style(column));
            }
        }
    }

    surface_pop_clip(surface);

    surface_pop_clip(surface);
}

static void reverse_array(int array[], int size)
{
    for (int i = 0; i < size / 2; i++)
    {
        int tmp = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = tmp;
    }
}

void model_view(User *user, const char *title, Model model, void *data)
{
    (void)title;
    ModelViewState state = {0};
    state.sort_dirty = true;

    terminal_enable_alternative_screen_buffer();

    Surface *surface = surface_create();

    do
    {
        surface_clear(surface, DEFAULT_STYLE);
        surface_update(surface);

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

        terminal_enter_rawmode();

        model_view_title(user, surface, title);
        model_view_status_bar(surface, &state, model, data);

        model_view_list(user, surface, &state, model, data);

        surface_pop_clip(surface);
        surface_pop_clip(surface);

        surface_render(surface);

        terminal_exit_rawmode();

        int codepoint = terminal_read_key();

        for (int i = 0; model.get_actions()[i].key_codepoint != 0; i++)
        {
            ModelAction action = model.get_actions()[i];

            if (action.key_codepoint == codepoint)
            {
                action.callback(user, surface, &state, model, data, state.sorted[state.slected]);
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

    } while (!state.exited);

    surface_destroy(surface);

    terminal_disable_alternative_screen_buffer();
}