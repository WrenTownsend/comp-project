#pragma once

typedef enum {
	//identifiers
	TK_ID,
	//keyword
	TK_FN,
	TK_VAR,
	TK_IF,
	TK_WHILE,
	TK_RETURN,
	//delimiter
	TK_SEMICOLON,
	TK_COMMA,
	TK_COLON,
	TK_DOT,
	TK_OPEN_PARAN,
	TK_CLOSE_PARAN,
	TK_OPEN_BRACE,
	TK_CLOSE_BRACE,
	TK_OPEN_BRACKET,
	TK_CLOSE_BRACKET,
	//operator
	TK_PLUS,
	TK_MINUS,
	TK_MULT,
	TK_DIV,

	TK_EQ,
	TK_GT,
	TK_LT,

	TK_ASSIGN,
	//literal
	TK_INT_LIT,
	//other
	TK_EOF,
	TK_NULL,
} token_type;

typedef struct {
	token_type type;
	char content[32];
	char* start;
	char* end; //incusive
} token_t;

typedef struct {
	char* file;
	char* parse_point;
	token_t token;
} lexer_t;

lexer_t* lexer_init(char*);
void lexer_reset(lexer_t*);
int get_token(lexer_t*);
int print_token(const token_t*);
