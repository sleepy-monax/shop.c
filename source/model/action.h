#pragma once

#include "model/user.h"

struct ModelViewState;
struct Model;

typedef void (*ModelActionCallback)(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);

typedef struct
{
    int key_codepoint;
    ModelActionCallback callback;

    const char *name;
    const char *desciption;
} ModelAction;

void quit_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);
void help_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);

void scroll_up_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);
void scroll_down_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);
void page_up_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);
void page_down_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);

void home_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);
void end_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);

void edit_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);
void create_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);
void delete_ModelActionCallback(User *user, Surface *surface, struct ModelViewState *state, struct Model model, void *data, int row);

#define DEFAULT_MODEL_VIEW_ACTION {'q', quit_ModelActionCallback, "Quitter", "Quitter l'inpecteur de model."},            \
                                  {'h', help_ModelActionCallback, "Aide", "Afficher l'aide"},                             \
                                  {'k', scroll_up_ModelActionCallback, "Scroll haut", "Scroller vers le haut."},          \
                                  {'j', scroll_down_ModelActionCallback, "Scroll base", "Scroller vers le base"},         \
                                  {'K', page_up_ModelActionCallback, "Page haut", "Scroller une page vers le haut."},     \
                                  {'J', page_down_ModelActionCallback, "Page bas", "Scroller une page vers le base."},    \
                                  {'g', home_ModelActionCallback, "Début", "Scroller tout en haut de la liste."},         \
                                  {'G', end_ModelActionCallback, "Fin", "Scroller tout en bas de la liste."},             \
                                  {'e', edit_ModelActionCallback, "Éditer", "Éditer l'élément actuelle."},                \
                                  {'i', create_ModelActionCallback, "Créer", "Créer un nouvelle élément dans la liste."}, \
                                  {'d', delete_ModelActionCallback, "Supprimer", "Supprimer l'élément selectioner de la liste"},

#define END_MODEL_VIEW_ACTION {0, NULL, NULL, NULL},