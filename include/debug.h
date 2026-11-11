#pragma once

#include "lexer.h"
#include "parser.h"
#include "rule_table.h"

void init_color();
int print_token(const token_t*);
int print_tree(node_t*);
int print_table(table_t*);
int print_rules(table_t*);
