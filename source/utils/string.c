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

size_t utf8len(const char *s)
{
    size_t count = 0;
    while (*s)
    {
        count += (*s++ & 0xC0) != 0x80;
    }
    return count;
}

uint32_t strhash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

int strutf8(uint8_t *out, Codepoint utf)
{
    if (utf <= 0x7F)
    {
        out[0] = (uint8_t)utf;
        out[1] = 0;

        return 1;
    }
    else if (utf <= 0x07FF)
    {
        out[0] = (uint8_t)(((utf >> 6) & 0x1F) | 0xC0);
        out[1] = (uint8_t)(((utf >> 0) & 0x3F) | 0x80);
        out[2] = 0;

        return 2;
    }
    else if (utf <= 0xFFFF)
    {
        out[0] = (uint8_t)(((utf >> 12) & 0x0F) | 0xE0);
        out[1] = (uint8_t)(((utf >> 6) & 0x3F) | 0x80);
        out[2] = (uint8_t)(((utf >> 0) & 0x3F) | 0x80);
        out[3] = 0;

        return 3;
    }
    else if (utf <= 0x10FFFF)
    {
        out[0] = (uint8_t)(((utf >> 18) & 0x07) | 0xF0);
        out[1] = (uint8_t)(((utf >> 12) & 0x3F) | 0x80);
        out[2] = (uint8_t)(((utf >> 6) & 0x3F) | 0x80);
        out[3] = (uint8_t)(((utf >> 0) & 0x3F) | 0x80);
        out[4] = 0;

        return 4;
    }
    else
    {
        out[0] = (uint8_t)0xEF;
        out[1] = (uint8_t)0xBF;
        out[2] = (uint8_t)0xBD;
        out[3] = 0;

        return 0;
    }
}

int utf8str(const uint8_t *in, Codepoint *out)
{
    if ((in[0] & 0xf8) == 0xf0)
    {
        *out = ((0x07 & in[0]) << 18) |
               ((0x3f & in[1]) << 12) |
               ((0x3f & in[2]) << 6) |
               ((0x3f & in[3]));

        return 4;
    }
    else if ((in[0] & 0xf0) == 0xe0)
    {
        *out = ((0x0f & in[0]) << 12) |
               ((0x3f & in[1]) << 6) |
               ((0x3f & in[2]));

        return 3;
    }
    else if ((in[0] & 0xe0) == 0xc0)
    {
        *out = ((0x1f & in[0]) << 6) |
               ((0x3f & in[1]));

        return 2;
    }
    else
    {
        *out = in[0];

        return 1;
    }

    return 0;
}