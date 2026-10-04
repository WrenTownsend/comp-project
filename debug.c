#include <stdio.h>
#include <unistd.h>
#include "debug.h"
#include "lexer.h"
#include "parser.h"

// color!!
static _Bool color_enabled;

#define RED(str) color_enabled ? "\x1b[31m"str"\x1b[0m" : str
#define GREEN(str) color_enabled ? "\x1b[32m"str"\x1b[0m" : str
#define YELLOW(str) color_enabled ? "\x1b[33m"str"\x1b[0m" : str
#define BLUE(str) color_enabled ? "\x1b[34m"str"\x1b[0m" : str
#define MAGENTA(str) color_enabled ? "\x1b[35m"str"\x1b[0m" : str
#define CYAN(str) color_enabled ? "\x1b[36m"str"\x1b[0m" : str

_Bool is_color_supported() {
	return isatty(fileno(stdout));
}

void init_color() {
	color_enabled = is_color_supported();
	return;
}

// ### lexer stuff ###
int print_token(const token_t* token) 
{
	switch(token->type) {
		//identifiers
		case TK_ID: printf("ID: %s\n", token->content); return 0;
		//keywords
		case TK_FN: printf("FN\n"); return 0;
		case TK_VAR: printf("VAR\n"); return 0;
		case TK_IF: printf("IF\n"); return 0;
		case TK_WHILE: printf("WHILE\n"); return 0;
		case TK_RETURN: printf("RETURN\n"); return 0;
		//delimiters
		case TK_SEMICOLON: printf("SEMICOLON\n"); return 0;
		case TK_COMMA: printf("COMMA\n"); return 0;
		case TK_COLON: printf("COLON\n"); return 0;
		case TK_DOT: printf("DOT\n"); return 0;
		case TK_OPEN_PARAN: printf("OPEN_PARAN\n"); return 0;
		case TK_CLOSE_PARAN: printf("CLOSE_PARAN\n"); return 0;
		case TK_OPEN_BRACE: printf("OPEN_BRACE\n"); return 0;
		case TK_CLOSE_BRACE: printf("CLOSE_BRACE\n"); return 0;
		case TK_OPEN_BRACKET: printf("OPEN_BRACKET\n"); return 0;
		case TK_CLOSE_BRACKET: printf("CLOSE_BRACKET\n"); return 0;
		//operator
		case TK_PLUS: printf("PLUS\n"); return 0;
		case TK_EQ: printf("EQ\n"); return 0;
		case TK_MULT: printf("MULT\n"); return 0;
		case TK_DIV: printf("DIV\n"); return 0;

		case TK_LT: printf("LT\n"); return 0;
		case TK_ASSIGN: printf("ASSIGN\n"); return 0;
		//literals
		case TK_INT_LIT: printf("INT_LIT: %s\n", token->content); return 0;
		//other
		case TK_EOF: printf("EOF\n"); return 0;
		case TK_NULL: printf("NULL\n"); return 0;
		default: printf("error: failed to print %d\n", token->type); return 1;
	}
}

// ### parser stuff ###
int print_node(const node_t* node)
{
	switch (node->type) {
		case N_PROGRAM: printf(GREEN("PROGRAM\n")); return 0;
		case N_TOP_ELEMS: printf("TOP_ELEMS\n"); return 0;
		case N_TOP_ELEM: printf("TOP_ELEM\n"); return 0;

		case N_FUNC_DEF: printf(BLUE("FUNC_DEF\n")); return 0;
		case N_FUNC_SIG: printf("FUNC_SIG\n"); return 0;

		case N_COMP_STMT: printf("COMP_STMT\n"); return 0;
		case N_STMTS: printf("STMTS\n"); return 0;
		case N_STMT: printf(MAGENTA("STMT\n")); return 0;

		case N_RETURN_STMT: printf("RETURN_STMT\n"); return 0;
		case N_DECL_STMT: printf("DECL_STMT\n"); return 0;
		case N_ASSIGN_STMT: printf("ASSIGN_STMT\n"); return 0;
		case N_IF_STMT: printf("IF_STMT\n"); return 0;
		case N_WHILE_STMT: printf("WHILE_STMT\n"); return 0;

		case N_EXPR: printf(RED("EXPR\n")); return 0;
		case N_TERM: printf("TERM\n"); return 0;
		case N_FACTOR: printf("FACTOR\n"); return 0;

		case N_PLUS: printf("PLUS\n"); return 0;
		case N_MINUS: printf("MINUS\n"); return 0;
		case N_MULT: printf("MULT\n"); return 0;
		case N_DIV: printf("DIV\n"); return 0;

		case N_ID: printf(YELLOW("ID\n")); return 0;
		case N_INT_LIT: printf(YELLOW("INT_LIT\n")); return 0;

		default: printf("error: failed to print %d\n", node->type); return 1;
	}
	return 0;
}

int print_tree_r(node_t* node, int level, int stack, int is_last)
{
	for(int i = 0; i < level; i++)
		if(i+1 == level) {
			is_last \
			? printf("%lc%lc%lc%lc%lc",0x2514,0x2500,0x2500,0x2500,0x2500) \
			: printf("%lc%lc%lc%lc%lc",0x251C,0x2500,0x2500,0x2500,0x2500);
		} else {
			if(((stack >> i) & 1) == 1)
				printf("%lc    ",0x2502);
			else
				printf("     ");
		}
	print_node(node);
	if(node->op.left) {
		print_tree_r(node->op.left, level+1,\
			node->op.right ? stack | 1 << level : stack, \
			node->op.right ? 0 : 1);
	}
	if(node->op.right) {
		print_tree_r(node->op.right, level+1, stack & ~(1 << level), 1 );
	}
	return 0;
}

int print_tree(node_t* node) {
	print_tree_r(node, 0, 0, 0);
	return 0;
}
