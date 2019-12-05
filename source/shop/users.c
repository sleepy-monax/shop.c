#include <string.h>
#include "shop/users.h"

User *users_lookup(UsersList *users, const char *login)
{
    list_foreach(item, users)
    {
        User *user = (User *)item->value;

        if (strcmp(login, user->login) == 0)
        {
            return user;
        }
    }

    return NULL;
}
