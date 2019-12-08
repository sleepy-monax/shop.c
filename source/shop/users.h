#pragma once

#include "utils/list.h"
#include "model/model.h"

typedef List UsersList;

Model users_model_create(void);

User *users_lookup(UsersList *users, const char *login);