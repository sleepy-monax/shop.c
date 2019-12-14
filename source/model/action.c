#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "model/view.h"
#include "utils/assert.h"
#include "utils/math.h"
#include "utils/terminal.h"

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

    bool editing = false;
    char edited_value[VARIANT_STRING_SIZE];
    int edited_offset;

    do
    {
        surface_clear(surface, DEFAULT_STYLE);
        model_view_title(NULL, surface, "Editer le modèle");
        model_view_status_bar(surface, state, model, data);

        for (int i = 0; i < model.column_count(); i++)
        {

            if (editing)
            {
                if (i == selected)
                {
                    char buffer[256];
                    snprintf(buffer, 256, "%16s: %s▂", model.column_name(i, ROLE_DATA), edited_value);
                    surface_text(surface, buffer, 0, i, surface_width(surface), DEFAULT_STYLE);
                }
                else
                {
                    char buffer[256];
                    snprintf(buffer, 256, "%16s: %s", model.column_name(i, ROLE_DATA), model_get_data_with_access(model, data, row, i, user, ROLE_DATA).as_string);
                    surface_text(surface, buffer, 0, i, surface_width(surface), DISABLED_DEFAULT_STYLE);
                }
            }
            else
            {
                char buffer[256];
                snprintf(buffer, 256, "%16s: %-16s", model.column_name(i, ROLE_DATA), model_get_data_with_access(model, data, row, i, user, ROLE_EDITOR).as_string);

                if (user->access <= model.write_access(data, row, i, user))
                {
                    if (i == selected)
                    {
                        surface_text(surface, buffer, 0, i, surface_width(surface), INVERTED_STYLE);
                    }
                    else
                    {
                        surface_text(surface, buffer, 0, i, surface_width(surface), DEFAULT_STYLE);
                    }
                }
                else
                {
                    if (i == selected)
                    {
                        surface_text(surface, buffer, 0, i, surface_width(surface), DISABLED_INVERTED_STYLE);
                    }
                    else
                    {
                        surface_text(surface, buffer, 0, i, surface_width(surface), DISABLED_DEFAULT_STYLE);
                    }
                }
            }
        }

        surface_pop_clip(surface);
        surface_pop_clip(surface);
        surface_render(surface);
        surface_update(surface);

        int key = terminal_read_key();

        if (editing)
        {
            if (key == '\n')
            {
                editing = false;
                if (model.column_type(selected, ROLE_EDITOR) == VARIANT_INT)
                {
                    model.set_data(data, row, selected, vint(atoi(edited_value)), ROLE_EDITOR);
                }
                else if (model.column_type(selected, ROLE_EDITOR) == VARIANT_FLOAT)
                {
                    float value;
                    sscanf(edited_value, "%f", &value);
                    model.set_data(data, row, selected, vfloat(value), ROLE_EDITOR);
                }
                else
                {
                    model.set_data(data, row, selected, vstring(edited_value), ROLE_EDITOR);
                }
            }
            else if (key == 127)
            {
                if (edited_offset > 0)
                {
                    edited_offset--;
                    edited_value[edited_offset] = '\0';
                }
            }
            else if (iscntrl(key))
            {
                // do nothing with it
            }
            else if (edited_offset < VARIANT_STRING_SIZE - 1)
            {
                switch (model.column_type(selected, ROLE_EDITOR))
                {
                case VARIANT_INT:
                    if (key >= '0' && key <= '9')
                    {
                        edited_value[edited_offset] = key;
                        edited_value[edited_offset + 1] = '\0';

                        edited_offset++;
                    }
                    break;

                case VARIANT_FLOAT:
                    if ((key >= '0' && key <= '9') || key == '.')
                    {
                        edited_value[edited_offset] = key;
                        edited_value[edited_offset + 1] = '\0';

                        edited_offset++;
                    }
                    break;

                case VARIANT_STRING:
                    edited_value[edited_offset] = key;
                    edited_value[edited_offset + 1] = '\0';

                    edited_offset++;
                    break;

                default:
                    ASSERT_NOT_REACHED();
                }
            }
        }
        else
        {
            if (key == 'j')
            {
                selected = min(selected + 1, model.column_count() - 1);
            }
            else if (key == 'k')
            {
                selected = max(selected - 1, 0);
            }
            else if (key == 'e')
            {
                if (user->access <= model.write_access(data, row, selected, user))
                {
                    editing = true;
                    strcpy(edited_value, model.get_data(data, row, selected, ROLE_EDITOR).as_string);
                    edited_offset = strlen(edited_value);
                }
            }
            else if (key == 'q')
            {
                exited = true;
            }
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