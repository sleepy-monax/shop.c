#include <string.h>

#include "utils/string.h"

bool str_start_with(const char *pre, const char *str)
{
    int lenpre = strlen(pre),
        lenstr = strlen(str);

    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}