#include "lexer.h"

typedef enum {
	N_PROGRAM,
	N_TOP_ELEMS,
	N_TOP_ELEM,

	N_FUNC_DEF,
	N_FUNC_SIG,

	N_COMP_STMT,
	N_STMTS,
	N_STMT,

	N_RETURN_STMT,
	N_DECL_STMT,
	N_ASSIGN_STMT,
	N_IF_STMT,
	N_WHILE_STMT,
	//output_stmt
	//input_stmt

	N_EXPR,
	N_TERM,
	N_FACTOR,

	N_PLUS,
	N_MINUS,
	N_MULT,
	N_DIV,

	N_ID,
	N_INT_LIT,
} node_type;

typedef struct node {
	node_type type;
	union {
		char content[32];
		struct {
			struct node* left;
			struct node* right;
		} op;
	};
} node_t;

node_t* get_syntax_tree(lexer_t*);
int print_tree(node_t*, int, int);
