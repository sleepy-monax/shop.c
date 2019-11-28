#include <stdio.h>
#include "shop/shop.h"

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
