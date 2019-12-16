#include <stdlib.h>
#include <string.h>

#include "shop/users.h"
#include "utils/assert.h"
#include "utils/string.h"

typedef enum
{
    COL_USERS_LOGIN,
    COL_USERS_LASTNAME,
    COL_USERS_FIRSTNAME,
    COL_USERS_PASSWORD,
    COL_USERS_ACCESS,

    __COL_USERS_COUNT,
} UserModelColumn;

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

ModelAccess users_ModelReadAccess(UsersList *users, int row, int column, User *user)
{
    if (list_indexof(users, user) == row)
    {
        return user->access;
    }
    else
    {
        switch (column)
        {
        case COL_USERS_LOGIN:
            return ACCESS_ADMIN;

        case COL_USERS_FIRSTNAME:
            return ACCESS_ALL;

        case COL_USERS_LASTNAME:
            return ACCESS_ALL;

        case COL_USERS_PASSWORD:
            return ACCESS_ADMIN;

        case COL_USERS_ACCESS:
            return ACCESS_ALL;
        }

        ASSERT_NOT_REACHED();
    }
}

ModelAccess users_ModelWriteAccess(UsersList *users, int row, int column, User *user)
{

    switch (column)
    {
    case COL_USERS_LOGIN:
        return ACCESS_ADMIN;

    case COL_USERS_FIRSTNAME:
        if (list_indexof(users, user) == row)
        {
            return ACCESS_ALL;
        }
        else
        {
            return ACCESS_MANAGER;
        }

    case COL_USERS_LASTNAME:
        if (list_indexof(users, user) == row)
        {
            return ACCESS_ALL;
        }
        else
        {
            return ACCESS_MANAGER;
        }

    case COL_USERS_PASSWORD:
        if (list_indexof(users, user) == row)
        {
            return ACCESS_ALL;
        }
        else
        {
            return ACCESS_MANAGER;
        }

    case COL_USERS_ACCESS:
        return ACCESS_ADMIN;
    }

    ASSERT_NOT_REACHED();
}

int users_ModelRowCount(UsersList *users)
{
    return list_count(users);
}

int users_ModelRowCreate(UsersList *users)
{
    User *new_user = malloc(sizeof(User));

    *new_user = (User){0};

    list_pushback(users, new_user);

    return list_count(users) - 1;
}

void users_ModelRowDelete(UsersList *users, int index)
{
    User *user_to_remove;
    list_peekat(users, index, (void **)&user_to_remove);
    list_remove(users, user_to_remove);
}

int users_ModelColumnCount(void)
{
    return __COL_USERS_COUNT;
}

const char *users_ModelColumnName(int index, ModelRole role)
{
    if (role == ROLE_DATA)
    {
        switch (index)
        {
        case COL_USERS_LOGIN:
            return "LOGIN";

        case COL_USERS_FIRSTNAME:
            return "FIRSTNAME";

        case COL_USERS_LASTNAME:
            return "LASTNAME";

        case COL_USERS_PASSWORD:
            return "PASSWORD";

        case COL_USERS_ACCESS:
            return "ACCESS";
        }
    }
    else
    {
        switch (index)
        {
        case COL_USERS_LOGIN:
            return "Login";

        case COL_USERS_FIRSTNAME:
            return "Prénom";

        case COL_USERS_LASTNAME:
            return "Nom";

        case COL_USERS_PASSWORD:
            return "Hash";

        case COL_USERS_ACCESS:
            return "Accès";
        }
    }

    ASSERT_NOT_REACHED();
}

VarianType users_ModelColumnType(int index, ModelRole role)
{
    switch (index)
    {
    case COL_USERS_LOGIN:
        return VARIANT_STRING;

    case COL_USERS_FIRSTNAME:
        return VARIANT_STRING;

    case COL_USERS_LASTNAME:
        return VARIANT_STRING;

    case COL_USERS_PASSWORD:
        if (role == ROLE_EDITOR)
        {
            return VARIANT_STRING;
        }
        else
        {
            return VARIANT_INT;
        }

    case COL_USERS_ACCESS:
        return VARIANT_INT;
    }

    ASSERT_NOT_REACHED();
}

Style users_ModelColumnStyle(int index)
{
    switch (index)
    {
    case COL_USERS_LOGIN:
        return style_centered(DEFAULT_STYLE);

    case COL_USERS_FIRSTNAME:
        return style_centered(DEFAULT_STYLE);

    case COL_USERS_LASTNAME:
        return style_centered(DEFAULT_STYLE);

    case COL_USERS_PASSWORD:
        return style_centered(DEFAULT_STYLE);

    case COL_USERS_ACCESS:
        return style_centered(DEFAULT_STYLE);
    }

    ASSERT_NOT_REACHED();
}

Variant users_ModelGetData(UsersList *users, int row, int column, ModelRole role)
{
    (void)role;

    User *user;
    list_peekat(users, row, (void **)&user);

    switch (column)
    {
    case COL_USERS_LOGIN:
        return vstring(user->login);

    case COL_USERS_FIRSTNAME:
        return vstring(user->firstname);

    case COL_USERS_LASTNAME:
        return vstring(user->lastname);

    case COL_USERS_PASSWORD:
        if (role == ROLE_EDITOR || role == ROLE_DISPLAY)
        {
            return vstring("*****");
        }
        else
        {
            return vint(user->password);
        }

    case COL_USERS_ACCESS:
        if (role == ROLE_DISPLAY)
        {
            switch (user->access)
            {
            case ACCESS_ADMIN:
                return vstring("Admin");

            case ACCESS_MANAGER:
                return vstring("Manager");

            case ACCESS_CASHIER:
                return vstring("Caissier");

            default:
                ASSERT_NOT_REACHED();
            }
        }
        else
        {
            return vint(user->access);
        }
    }

    ASSERT_NOT_REACHED();
}

void users_ModelSetData(UsersList *users, int row, int column, Variant value, ModelRole role)
{
    User *user;
    list_peekat(users, row, (void **)&user);
    assert(user);

    switch (column)
    {
    case COL_USERS_LOGIN:
        strcpy(user->login, value.as_string);

        break;

    case COL_USERS_FIRSTNAME:
        strcpy(user->firstname, value.as_string);
        break;

    case COL_USERS_LASTNAME:
        strcpy(user->lastname, value.as_string);
        break;

    case COL_USERS_PASSWORD:
        if (role == ROLE_EDITOR)
        {
            if (strcmp(value.as_string, "*****") != 0)
            {
                user->password = strhash((const uint8_t *)value.as_string);
            }
        }
        else
        {
            user->password = value.as_int;
        }
        break;

    case COL_USERS_ACCESS:
        user->access = value.as_int;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
}

ModelAction users_actions[] = {DEFAULT_MODEL_VIEW_ACTION END_MODEL_VIEW_ACTION};

ModelAction *users_ModelGetActions(void)
{
    return users_actions;
}

Model users_model_create(void)
{
    return (Model){
        (ModelReadAccess)users_ModelReadAccess,
        (ModelWriteAccess)users_ModelWriteAccess,

        (ModelRowCount)users_ModelRowCount,
        (ModelRowCreate)users_ModelRowCreate,
        (ModelRowDelete)users_ModelRowDelete,

        (ModelColumnCount)users_ModelColumnCount,
        (ModelColumnName)users_ModelColumnName,
        (ModelColumnType)users_ModelColumnType,
        (ModelColumnStyle)users_ModelColumnStyle,

        (ModelGetData)users_ModelGetData,
        (ModelSetData)users_ModelSetData,

        (ModelGetActions)users_ModelGetActions,
    };
}
