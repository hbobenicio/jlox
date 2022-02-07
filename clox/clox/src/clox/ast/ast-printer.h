#ifndef CLOX_AST_PRINTER_H
#define CLOX_AST_PRINTER_H

#include <stdio.h>

struct expr;

void ast_printer_println(struct expr* expr);
void ast_printer_fprintln(FILE* file, struct expr* expr);

#endif
