#ifndef CLOX_AST_PRINTER_H
#define CLOX_AST_PRINTER_H

#include <stdio.h>

struct expr;

struct ast_printer {
    FILE* file;
};

void ast_printer_print(struct ast_printer* ast_printer, struct expr* expr);

#endif
