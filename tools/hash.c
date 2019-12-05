#include <stdio.h>
#include <stdint.h>

uint32_t strhash(const unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

int main(int argc, char const *argv[])
{
    printf("%u\n", strhash((const uint8_t *)argv[1]));
    return 0;
}
