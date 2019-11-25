#include "utils/string.h"
#include "view/views.h"

void autocomplete_client_list(const char *user_input, ClientsList *clients)
{
    list_foreach(item, clients)
    {
        Client *client = (Client *)item->value;

        char client_id_string[5];
        sprintf(client_id_string, "%03d", client->id);

        if (str_start_with(user_input, client_id_string))
        {
            printf("%04d %s %s\n", client->id, client->firstname, client->lastname);
        }
    }
}