#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "utils/assert.h"
#include "utils/string.h"
#include "utils/variant.h"
#include "utils/logger.h"

Variant vint(long int value)
{
    Variant v = (Variant){
        .type = VARIANT_INT,
        .as_int = value,
    };

    snprintf(v.as_string, VARIANT_STRING_SIZE, "%ld", value);

    return v;
}

Variant vfloat(float value)
{
    Variant v = (Variant){
        .type = VARIANT_FLOAT,
        .as_float = value,
    };

    snprintf(v.as_string, VARIANT_STRING_SIZE, "%.2f", value);

    return v;
}

Variant vstring(const char *value)
{
    Variant v = (Variant){.type = VARIANT_STRING};

    assert(strlen(value) < VARIANT_STRING_SIZE);

    strncpy(v.as_string, value, VARIANT_STRING_SIZE);

    return v;
}

Variant vstringf(const char *fmt, ...)
{
    Variant v = (Variant){.type = VARIANT_STRING};

    va_list args;
    va_start(args, fmt);
    vsnprintf(v.as_string, VARIANT_STRING_SIZE, fmt, args);
    va_end(args);

    return v;
}

int variant_cmp(Variant left, Variant right)
{
    if (left.type == VARIANT_INT && right.type == VARIANT_INT)
    {
        return left.as_int - right.as_int;
    }
    else if (left.type == VARIANT_FLOAT && right.type == VARIANT_FLOAT)
    {
        return left.as_float - right.as_float;
    }
    else
    {
        return strcmp(left.as_string, right.as_string);
    }
}

Variant variant_deserialize(const char *source)
{
    Variant value = vint(-69420);

    if (source[0] == '"')
    {
        char buffer[VARIANT_STRING_SIZE] = {0};

        bool escaped = true;

        for (int i = 1; source[i]; i++)
        {
            char c = source[i];

            if (c == '\\' && !escaped)
            {
                escaped = true;
            }
            else if (escaped || (c != '\\' && c != '"'))
            {
                strnapd(buffer, c, VARIANT_STRING_SIZE);

                escaped = false;
            }
        }

        value = vstring(buffer);
    }
    else if (str_is_int(source))
    {
        long int v;
        sscanf(source, "%ld", &v);
        value = vint(v);
    }
    else if (str_is_float(source))
    {
        float v;
        sscanf(source, "%f", &v);
        value = vfloat(v);
    }

    return value;
}

void variant_serialize(Variant value, char *destination)
{
    destination[0] = '\0';

    switch (value.type)
    {
    case VARIANT_INT:
        sprintf(destination, "%ld", value.as_int);
        break;

    case VARIANT_FLOAT:
        sprintf(destination, "%f", value.as_float);
        break;

    case VARIANT_STRING:
        strnapd(destination, '"', VARIANT_SERIALIZED_SIZE);

        for (int i = 0; value.as_string[i]; i++)
        {
            char c = value.as_string[i];

            if (c == '"' || c == '\\')
            {
                strnapd(destination, '\\', VARIANT_SERIALIZED_SIZE);
            }

            strnapd(destination, c, VARIANT_SERIALIZED_SIZE);
        }

        strnapd(destination, '"', VARIANT_SERIALIZED_SIZE);
        break;

    default:
        ASSERT_NOT_REACHED();
    }
}