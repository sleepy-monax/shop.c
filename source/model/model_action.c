#include "model/model_view.h"
#include "utils/terminal.h"

void quit_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)model, (void)data, (void)row;

    state->exited = true;
}

void help_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    model_view_draw_title("💡 Rubrique d'aide", state->width);
    terminal_clear();

    for (int i = 0; model.get_actions()[i].key_codepoint != 0; i++)
    {
        ModelAction action = model.get_actions()[i];

        printf("\n\t • [%c] %s - %s ", action.key_codepoint, action.name, action.desciption);
    }

    model_view_draw_status_bar(*state, model, data, "💡 Appuyer sur n'importe quelle touche pour continuer...");

    termianl_read_key();
}

void scroll_up_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)model, (void)data, (void)row;

    state->slected--;
}

void scroll_down_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    state->slected++;
}

void page_up_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    state->slected -= 10;
}

void page_down_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    state->slected += 10;
}

void home_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    state->slected = 0;
}

void end_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    state->slected = model.row_count(data);
}

void edit_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    model_view_edit(*state, model, data, row);
    state->sort_dirty = true;
}

void create_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    model_view_edit(*state, model, data, model.row_create(data));
    state->sort_dirty = true;
}

void delete_ModelActionCallback(int codepoint, ModelViewState *state, Model model, void *data, int row)
{
    (void)codepoint, (void)state, (void)model, (void)data, (void)row;

    model.row_delete(data, state->sorted[state->slected]);
    state->sort_dirty = true;
}