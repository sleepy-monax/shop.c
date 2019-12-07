#include <string.h>
#include <stdlib.h>

#include "shop/users.h"
#include "utils/assert.h"

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

VarianType users_ModelColumnType(int index)
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
        return VARIANT_INT;

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
        return DEFAULT_STYLE;

    case COL_USERS_ACCESS:
        return DEFAULT_STYLE;
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
        return vint(user->password);

    case COL_USERS_ACCESS:
        return vint(user->access);
    }

    ASSERT_NOT_REACHED();
}

void users_ModelSetData(UsersList *users, int row, int column, Variant value)
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
        user->password = value.as_int;
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
