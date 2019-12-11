#pragma once

typedef enum
{
    ACCESS_NONE = -1,
    ACCESS_ADMIN,
    ACCESS_MANAGER,
    ACCESS_CASHIER,
    ACCESS_ALL,
} ModelAccess;

#define USER_FIRST_NAME_SIZE 64
#define USER_LAST_NAME_SIZE 64

typedef struct
{
    char login[8];
    char firstname[USER_FIRST_NAME_SIZE];
    char lastname[USER_LAST_NAME_SIZE];

    long int password;
    ModelAccess access;
} User;
