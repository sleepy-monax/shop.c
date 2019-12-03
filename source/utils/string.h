#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint32_t Codepoint;

void strnapd(char *str, char c, size_t n);

bool str_start_with(const char *pre, const char *str);

bool is_numeric(int c);

bool is_letter(int c);

bool is_white_space(int c);

bool str_is_int(const char *str);

bool str_is_float(const char *str);

size_t utf8len(const char *s);

uint32_t strhash(unsigned char *str);

int strutf8(uint8_t *out, Codepoint utf);

int utf8str(uint8_t *in, Codepoint *out);
