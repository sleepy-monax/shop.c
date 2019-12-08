#include <stdlib.h>
#include <unistd.h>

#include "utils/input.h"
#include "utils/logger.h"
#include "view/views.h"

#include "utils/terminal.h"

static void login_client(Session *session, ClientsList *clients)
{
    BareCode extra_barecode;

    do
    {
        extra_barecode = barecode_input("Inserez votre code " EXTRA);
        session->client = clients_lookup(clients, extra_barecode);

        log_info("Connection avec " EXTRA "#%04d...", extra_barecode);

        if (session->client == NULL)
        {
            log_error("Ce compte n'existe pas");

            if (user_yes_no("Erreur, identifiant incorrect, voulez-vous reessayer ? ") == NO)
            {
                return;
            }
        }
    } while (session->client == NULL);
}

static void new_client(Session *session, ClientsList *clients)
{
    BareCode nouveau_client_id = clients_generate_id(clients);
    Client *nouveau_client = (Client *)malloc(sizeof(Client));

    terminal_enable_alternative_screen_buffer();

    do
    {
        user_input("Inserez votre nom ", "......................", nouveau_client->lastname);
        user_input("Inserez votre prenom ", "......................", nouveau_client->firstname);
        user_input("Inserez votre email ", "********************************", nouveau_client->email);

        printf("\nVos informations :\n"

               "\tId: %d\n"
               "\tNom: %s\n"
               "\tPrenom: %s\n"
               "\tEmail: %s\n\n",

               nouveau_client_id,
               nouveau_client->firstname,
               nouveau_client->lastname,
               nouveau_client->email);

    } while (user_yes_no("Confirmer ?") == NO);

    terminal_disable_alternative_screen_buffer();

    nouveau_client->id = nouveau_client_id;
    nouveau_client->points = 0;

    list_pushback(clients, nouveau_client);

    session->client = nouveau_client;
}

void cashier_input_card_id(Session *session, ClientsList *clients)
{
    const char *prompt = "Cher client, vous pouvez profiter des points extra grace au compte " EXTRA " Colruyt";

    const char *choices[] = {
        "S'authentifier",
        "Créer un compte client Colruyt",
        "Continuer sans compte",
        NULL,
    };

    switch (user_select(NULL, prompt, choices))
    {
    case 0:
        login_client(session, clients);
        break;

    case 1:
        if (user_yes_no("Acceptez-vous les condition d'utilisation?") == YES)
            new_client(session, clients);
        break;

    case 2:
        log_info("Vous continuez sans compte...");
        break;
    }
}