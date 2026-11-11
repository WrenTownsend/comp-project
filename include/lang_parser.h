#pragma once

#include "ebnf_parser.h"
#include "lexer.h"

int check_syntax(lexer_t* lexer, table_t* table, rule_c* rule);

