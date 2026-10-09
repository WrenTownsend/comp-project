#pragma once

#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"

void init_color();
int print_token(const token_t*);
int print_tree(node_t*);
int print_table(table_t*);
