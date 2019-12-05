#pragma once

#include "utils/list.h"
#include "model/model.h"

#define USER_FIRST_NAME_SIZE 64
#define USER_LAST_NAME_SIZE 64

typedef enum
{
    USER_ADMIN,
    USER_MANAGER,
    USER_CASHIER,
} UserAccess;

typedef struct
{
    char login[8];
    char firstname[USER_FIRST_NAME_SIZE];
    char lastname[USER_LAST_NAME_SIZE];

    long int password;
    UserAccess access;
} User;

typedef List UsersList;

Model users_model_create(void);

User *users_lookup(UsersList *users, const char *login);