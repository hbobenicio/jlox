#ifndef CLOX_AST_PRINTER_H
#define CLOX_AST_PRINTER_H

#include <stdio.h>

struct clox_ast_expr;

void ast_printer_println(struct clox_ast_expr* expr);
void ast_printer_fprintln(FILE* file, struct clox_ast_expr* expr);

#endif
