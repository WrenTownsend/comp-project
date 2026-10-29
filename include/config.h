#pragma once

// ### LEXER ###

// ### PARSER ###

// Tagged union for elements
typedef enum {
	RULE,
	TERMINAL,
	LITERAL,
} c_tag;
typedef struct {
	c_tag type;
	union {
		c_rule rule;
		c_terminal terminal;
		c_literal literal;
	} data;
} c_element;

typedef struct {
	char name[32];
	int size;
	c_element operands[32];
} c_rule;
typedef struct {
	int size;
	char string[32];
} c_literal;
typedef struct {

} c_terminal;

// List of rules
typedef struct {
	int size;
	int capacity;
	c_rule* arr;
} rules_list;

