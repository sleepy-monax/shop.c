#include "model/view.h"
#include "utils/terminal.h"
#include "utils/math.h"

void quit_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)model, (void)data, (void)row;

    state->exited = true;
}

void help_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    surface_clear(surface, DEFAULT_STYLE);

    model_view_title(NULL, surface, "Rubrique d'aide");

    for (int i = 0; model.get_actions()[i].key_codepoint != 0; i++)
    {
        ModelAction action = model.get_actions()[i];
        char buffer[256];

        snprintf(buffer, 256, "• [%c] %s - %s", action.key_codepoint, action.name, action.desciption);
        surface_text(surface, buffer, 2, i, surface_width(surface), DEFAULT_STYLE);
    }

    surface_pop_clip(surface);

    surface_render(surface);

    terminal_read_key();
}

void scroll_up_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)model, (void)data, (void)row;

    state->slected--;
}

void scroll_down_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model, void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    state->slected++;
}

void page_up_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    state->slected -= 10;
}

void page_down_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    state->slected += 10;
}

void home_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    state->slected = 0;
}

void end_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    state->slected = model.row_count(data);
}

void edit_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)surface, (void)state, (void)model, (void)data, (void)row;

    bool exited = false;
    int selected = 0;

    do
    {
        surface_clear(surface, DEFAULT_STYLE);
        model_view_title(NULL, surface, "Editer le model");
        model_view_status_bar(surface, state, model, data);

        for (int i = 0; i < model.column_count(); i++)
        {
            char buffer[256];

            snprintf(buffer, 256, "%16s: %-16s", model.column_name(i, ROLE_DATA), model_get_data_with_access(model, data, row, i, user, ROLE_DATA).as_string);

            if (user->access <= model.write_access(data, row, i, user))
            {
                if (i == selected)
                {
                    surface_text(surface, buffer, 2, i, surface_width(surface), INVERTED_STYLE);
                }
                else
                {
                    surface_text(surface, buffer, 2, i, surface_width(surface), DEFAULT_STYLE);
                }
            }
            else
            {
                if (i == selected)
                {
                    surface_text(surface, buffer, 2, i, surface_width(surface), DISABLED_INVERTED_STYLE);
                }
                else
                {
                    surface_text(surface, buffer, 2, i, surface_width(surface), DISABLED_DEFAULT_STYLE);
                }
            }
        }

        surface_pop_clip(surface);
        surface_pop_clip(surface);
        surface_render(surface);

        int key = terminal_read_key();

        if (key == 'j')
        {
            selected = min(selected + 1, model.column_count() - 1);
        }
        else if (key == 'k')
        {
            selected = max(selected - 1, 0);
        }
        else if (key == 'q')
        {
            exited = true;
        }
    } while (!exited);

    state->sort_dirty = true;
}

void create_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    int new_row = model.row_create(data);
    edit_ModelActionCallback(user, surface, state, model, data, new_row);
    state->sort_dirty = true;
}

void delete_ModelActionCallback(
    User *user,
    Surface *surface,
    ModelViewState *state,
    Model model,
    void *data,
    int row)
{
    (void)user, (void)surface, (void)state, (void)model, (void)data, (void)row;

    model.row_delete(data, state->sorted[state->slected]);
    state->sort_dirty = true;
}