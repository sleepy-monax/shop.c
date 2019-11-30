#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "utils/variant.h"

Variant variant_create_from_int(int value)
{
    Variant v = (Variant){
        .type = VARIANT_INT,
        .as_int = value,
    };

    snprintf(v.as_string, 128, "%d", value);

    return v;
}

Variant variant_create_from_float(float value)
{
    Variant v = (Variant){
        .type = VARIANT_FLOAT,
        .as_float = value,
    };

    snprintf(v.as_string, 128, "%f", value);

    return v;
}

Variant variant_create_from_string(const char *value)
{
    Variant v = (Variant){.type = VARIANT_STRING};

    assert(strlen(value) < 128);

    strcpy(v.as_string, value);

    return v;
}