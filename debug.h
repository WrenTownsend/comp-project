#pragma once

#include "lexer.h"
#include "parser.h"

void init_color();
int print_token(const token_t*);
int print_tree(node_t*);
