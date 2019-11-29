#include <stdio.h>
#include <stdlib.h>

#include "shop/shop.h"
#include "utils/input.h"

const char *barecode_char[] = {
    "█",
    "▉",
    "▊",
    "▋",
    "▌",
    "▍",
    "▎",
    "▕",
    "▏",
    "|",
};

void barecode_print(BareCode barecode)
{
    char buffer[5];
    sprintf(buffer, "%04d", barecode);

    printf("\e[47;30m");
    for (int i = 0; i < 4; i++)
    {
        printf("%s", barecode_char[buffer[i] - '0']);
    }

    for (int i = 0; i < 4; i++)
    {
        printf("%s", barecode_char[buffer[i] - '0']);
    }
    printf("\e[0m");
}

BareCode barecode_input(const char *prompt)
{
    char raw_barecode[5];
    user_input(prompt, "####", raw_barecode);
    return (BareCode)atoi(raw_barecode);
}
