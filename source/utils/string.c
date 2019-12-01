#include <string.h>

#include "utils/string.h"

void strnapd(char *str, char c, size_t n)
{
    for (size_t i = 0; i < (n - 1); i++)
    {
        if (str[i] == '\0')
        {
            str[i] = c;
            str[i + 1] = '\0';
            return;
        }
    }
}

bool str_start_with(const char *pre, const char *str)
{
    int lenpre = strlen(pre),
        lenstr = strlen(str);

    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

bool is_numeric(int c)
{
    return c == '.' || (c >= '0' && c <= '9');
}

bool is_letter(int c)
{
    return c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool is_white_space(int c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool str_is_int(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!is_numeric(str[i]) || str[i] == '.')
        {
            return false;
        }
    }

    return true;
}

bool str_is_float(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!is_numeric(str[i]))
        {
            return false;
        }
    }

    return true;
}

size_t strlen_unicode(const char *s)
{
    size_t count = 0;
    while (*s)
    {
        count += (*s++ & 0xC0) != 0x80;
    }
    return count;
}

unsigned long strhash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}