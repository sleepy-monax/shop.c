#pragma once

#define EXTRA "\e[31mEXTRA\e[0m"

typedef int BareCode;

void barecode_print(BareCode barecode);

BareCode barecode_input(const char *prompt);