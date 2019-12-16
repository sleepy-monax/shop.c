#include <unistd.h>

#include "utils/input.h"
#include "view/views.h"

void user_login(UsersList *users, StockList *stocks, ClientsList *clients)
{
    do
    {
        char user_login[17];
        user_input("\n\tLogin", "****************", user_login);

        char user_password[17];
        user_input_password("\tPassword", user_password, 17);
        long int hash = strhash((uint8_t *)user_password);

        User *user = users_lookup(users, user_login);

        if (user != NULL && hash == user->password)
        {
            home_select_what_todo(user, users, stocks, clients);
            return;
        }
        else
        {
            sleep(3);
            printf("\n\e[35mMot de passe ou nom d'utilisateur incorrect!\e[0m\n");
        }
    } while (true);
}
