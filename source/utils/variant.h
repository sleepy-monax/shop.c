#pragma once

#define VARIANT_STRING_SIZE 64
#define VARIANT_SERIALIZED_SIZE 128

typedef enum
{
    VARIANT_INT,
    VARIANT_FLOAT,
    VARIANT_STRING,
} VarianType;

typedef struct
{
    VarianType type;

    union {
        int as_int;
        float as_float;
    };

    char as_string[VARIANT_STRING_SIZE];
} Variant;

#define vint(__value) variant_create_from_int((__value))

#define vfloat(__value) variant_create_from_float((__value))

#define vstring(__value) variant_create_from_string((__value))

Variant variant_create_from_int(int value);

Variant variant_create_from_float(float value);

Variant variant_create_from_string(const char *value);

int variant_cmp(Variant left, Variant right);

Variant variant_deserialize(const char *source);

void variant_serialize(Variant value, char *destination);
