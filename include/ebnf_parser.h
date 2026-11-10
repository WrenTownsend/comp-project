#pragma once

#include "lexer.h"
#include "rule_table.h"

// ### LEXER ###

// ### PARSER ###
typedef enum tag_c tag_c;
typedef struct element_c element_c;
typedef struct option_c option_c;
typedef struct rule_c rule_c;

enum tag_c {
	RULE,
	TERMINAL,
	NOTHING,
}; 

struct element_c {
	tag_c type;
	union {
		token_type terminal;
		rule_c** rule; //points to the entry in the rule table
	} data;
	element_c* next;
};

struct option_c {
	element_c* element_list;
	option_c* next;
};

struct rule_c {
	option_c* option_list;
};

// functions
int parse_ebnf(lexer_t*, table_t*);
