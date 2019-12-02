#pragma once

#include <stdbool.h>
#include <stddef.h>

void strnapd(char *str, char c, size_t n);

bool str_start_with(const char *pre, const char *str);

bool is_numeric(int c);

bool is_letter(int c);

bool is_white_space(int c);

bool str_is_int(const char *str);

bool str_is_float(const char *str);

size_t strlen_unicode(const char *s);

unsigned long strhash(unsigned char *str);
