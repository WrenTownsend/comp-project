#include <stdlib.h>
#include <stdio.h>
// #include <wchar.h> //for utf-8
#include "parser.h"
#include "lexer.h"

node_t* program(lexer_t* l);
node_t* top_elems(lexer_t* l);
node_t* top_elem(lexer_t* l);

node_t* func_def(lexer_t* l);
node_t* func_sig(lexer_t* l);

node_t* comp_stmt(lexer_t* l);
node_t* stmts(lexer_t* l);
node_t* stmt(lexer_t* l);

node_t* return_stmt(lexer_t* l);
node_t* decl_stmt(lexer_t* l);
node_t* assign_stmt(lexer_t* l);
node_t* if_stmt(lexer_t* l);
node_t* while_stmt(lexer_t* l);

node_t* expr(lexer_t* l);
node_t* term(lexer_t* l);
node_t* factor(lexer_t* l);

node_t* id(lexer_t* l);
node_t* int_lit(lexer_t* l);

// ### new nodes ###

node_t* new_node(node_type type, node_t* left, node_t* right)
{
	node_t* node = calloc(1, sizeof(node_t));
	node->type = type;
	node->op.left = left;
	node->op.right = right;
	return node;
}
node_t* new_terminal(node_type type)
{
	node_t* node = calloc(1, sizeof(node_t));
	node->type = type;
	// strcpy(node->content, content);
	return node;
}

// ### top level stuff ###

//<program> -> <top_elems> "EOF"
node_t* program(lexer_t* l)
{
	node_t* left = top_elems(l);
	if(!left) {
		return NULL;
	}

	if(l->token.type != TK_EOF) {
		printf("error: expected EOF after <top_emels> in <program>");
		exit(1);
	}
	get_token(l);

	return new_node(N_PROGRAM, left, NULL);
}

//<top_elems> -> <top_elem> <top_elems>
//	       | <top_elem>
node_t* top_elems(lexer_t* l) {
	node_t* left = top_elem(l);
	if (!left) {
		return NULL;
	}

	node_t* right = top_elems(l);
	if (!right) {
		return new_node(N_TOP_ELEMS, left, NULL);
	}

	return new_node(N_TOP_ELEMS, left, right);
}

//<top_elem> -> <func_def> | <top_decl>
node_t* top_elem(lexer_t* l)
{
	node_t* left = func_def(l);
	if(left) {
		return new_node(N_TOP_ELEM, left, NULL);
	}

	// TODO: implement <top_decl>

	return NULL;
}

// ### function stuff ###

//<func_def> -> <func_sig> <comp_stmt>
node_t* func_def(lexer_t* l)
{
	node_t* left = func_sig(l);
	if(!left) {
		return NULL;
	}

	node_t* right = comp_stmt(l);
	if(!right) {
		printf("error: expected <comp_stmt> after <func_sig> in <func_def>\n");
		exit(1);
	}

	return new_node(N_FUNC_DEF, left, right);
}
//<func_sig> -> "fn" ID "(" ")" TODO: implement <id_list>
node_t* func_sig(lexer_t* l)
{
	if(l->token.type != TK_FN) {
		return NULL;
	}
	get_token(l);

	node_t* left = id(l);
	if(!left) {
		printf("error: expected ID after \"fn\" in <func_sig>\n");
		exit(1);
	}

	return new_node(N_FUNC_SIG, left, NULL);
}

// ### statments ###

//<comp_stmt> -> "{" <stmts> "}"
node_t* comp_stmt(lexer_t* l) {
	if(l->token.type != TK_OPEN_BRACE) {
		return NULL;
	}
	get_token(l);

	node_t* left = stmts(l);
	if(!left) {
		printf("error: expected <stmts> in <comp_stmt>\n");
		exit(1);
	}

	if(l->token.type != TK_CLOSE_BRACE) {
		printf("error: expected \"}\" at end of <comp_stmt>\n");
		exit(1);
	}
	get_token(l);

	return new_node(N_COMP_STMT, left, NULL);
}
//<stmts> -> <stmt> <stmts>
//	   | <stmt>
node_t* stmts(lexer_t* l) {
	node_t* left = stmt(l);
	if (!left) {
		return NULL;
	}

	node_t* right = stmts(l);
	if (!right) {
		return new_node(N_STMTS, left, NULL);
	}

	return new_node(N_STMTS, left, right);
}

//<stmt> -> <decl_stmt> | <if_stmt> | ...
node_t* stmt(lexer_t* l) {
	node_t* left;

	left = return_stmt(l);
	if (left) {
		return new_node(N_STMT, left, NULL);
	}

	left = decl_stmt(l);
	if (left) {
		return new_node(N_STMT, left, NULL);
	}

	left = assign_stmt(l);
	if (left) {
		return new_node(N_STMT, left, NULL);
	}

	left = if_stmt(l);
	if (left) {
		return new_node(N_STMT, left, NULL);
	}

	left = while_stmt(l);
	if (left) {
		return new_node(N_STMT, left, NULL);
	}

	return NULL;
}

// ### types of statments ###

node_t* return_stmt(lexer_t* l) {
	if(l->token.type != TK_RETURN) {
		return NULL;
	}
	get_token(l);

	if(l->token.type != TK_SEMICOLON) {
		printf("error: expected \";\" after \"return\" in <return_stmt>\n");
		exit(1);
	}
	get_token(l);

	return new_terminal(N_RETURN_STMT);
}

//<decl_stmt> -> "var" "id" ";"
node_t* decl_stmt(lexer_t* l) {
	if(l->token.type != TK_VAR) {
		return NULL;
	}
	get_token(l);

	node_t* left = id(l);
	if(!left) {
		printf("error: expected ID after \"var\"\n");
		exit(1);
	}

	if(l->token.type != TK_SEMICOLON) {
		printf("error: expected SEMICOLON after <decl_stmt>");
		exit(1);
	}
	get_token(l);

	return new_node(N_DECL_STMT, left, NULL);
}
//<assign_stmt> -> "id" "=" <expr> ";"
node_t* assign_stmt(lexer_t* l) {
	node_t* left = id(l);
	if(!left) {
		return NULL;
	}

	if(l->token.type != TK_ASSIGN) {
		printf("error: expected \"=\" after ID in <assign_stmt>\n");
		exit(1);
	}
	get_token(l);

	node_t* right = expr(l);
	if(!right) {
		printf("error: expected <expr> after \"=\" in <assign_stmt>\n");
		exit(1);
	}

	if(l->token.type != TK_SEMICOLON) {
		printf("error: expected \";\" after <expr> in <assign_stmt>\n");
		exit(1);
	}
	get_token(l);

	return new_node(N_ASSIGN_STMT, left, right);
}
//<if_stmt> -> "if" "(" <expr> ")" <comp_stmt>
node_t* if_stmt(lexer_t* l) {
	if(l->token.type != TK_IF) {
		return NULL;
	}
	get_token(l);

	if(l->token.type != TK_OPEN_PARAN) {
		printf("error: expected \"(\" after \"if\" in <if_stmt>\n");
		exit(1);
	}
	get_token(l);

	node_t* left = expr(l);
	if(!left) {
		printf("error: expected <expr> after \"(\" in <if_stmt>\n");
		exit(1);
	}

	if(l->token.type != TK_CLOSE_PARAN) {
		printf("error: expected \")\" after <expr> in <if_stmt>\n");
		exit(1);
	}
	get_token(l);

	node_t* right = comp_stmt(l);
	if(!left) {
		printf("error: expected <comp_stmt> after \")\" in <if_stmt>\n");
		exit(1);
	}

	return new_node(N_IF_STMT, left, right);
}
//<while_stmt> -> "while" "(" <expr> ")" <comp_stmt>
node_t* while_stmt(lexer_t* l) {
	if(l->token.type != TK_WHILE) {
		return NULL;
	}
	get_token(l);

	if(l->token.type != TK_OPEN_PARAN) {
		printf("error: expected \"(\" after \"while\" in <while_stmt>\n");
		exit(1);
	}
	get_token(l);

	node_t* left = expr(l);
	if(!left) {
		printf("error: expected <expr> after \"(\" in <while_stmt>\n");
		exit(1);
	}

	if(l->token.type != TK_CLOSE_PARAN) {
		printf("error: expected \")\" after <expr> in <while_stmt>\n");
		exit(1);
	}
	get_token(l);

	node_t* right = comp_stmt(l);
	if(!left) {
		printf("error: expected <comp_stmt> after \")\" in <while_stmt>\n");
		exit(1);
	}

	return new_node(N_WHILE_STMT, left, right);
}
//<output_stmt>
//<input_stmt>

// ### expression ###

//<expr> -> <term>{("+"|"-")<term>}
node_t* expr(lexer_t* l) {
	node_t* left = term(l);
	if(!left) {
		return NULL;
	}
	for(;;) {
		if (l->token.type == TK_PLUS) {
			get_token(l);
			node_t* right = term(l);
			if(!right) {
				printf("error: expected TERM after \"+\"\n");
				exit(1);
			};
			left = new_node(N_PLUS, left, right);
		} else if (l->token.type == TK_MINUS) {
			get_token(l);
			node_t* right = term(l);
			if(!right) {
				printf("error: expected TERM after \"-\"\n");
				exit(1);
			}
			left = new_node(N_MINUS, left, right);
		} else {
			return new_node(N_EXPR, left, NULL);
		}
	}
}

//<term> -> <factor>{("*"|"/")<factor>}
node_t* term(lexer_t* l) {
	node_t* left = factor(l);
	if(!left) {
		return NULL;
	};
	for(;;) {
		if (l->token.type == TK_MULT) {
			get_token(l);
			node_t* right = factor(l);
			if(!right) {
				printf("error: expected <factor> after \"*\"\n");
				exit(1);
			}
			left = new_node(N_MULT, left, right);
		} else if (l->token.type == TK_DIV) {
			get_token(l);
			node_t* right = factor(l);
			if(!right) {
				printf("error: expected <factor> after \"*\"\n");
				exit(1);
			}
			left = new_node(N_DIV, left, right);
		} else {
			return new_node(N_TERM, left, NULL);
		}
	}
}

//<factor> -> "id" | "int_lit" | "(" <expr> ")"
node_t* factor(lexer_t* l) {
	node_t* left;
	
	left = id(l);
	if (left) {
		return new_node(N_FACTOR, left, NULL);
	}

	left = int_lit(l);
	if (left) {
		return new_node(N_FACTOR, left, NULL);
	}

	if (l->token.type == TK_OPEN_PARAN) {
		get_token(l);
		node_t* left = expr(l);
		if(!left) {
			printf("error: expected <expr> after \"(\"\n");
			exit(1);
		}
		if (l->token.type != TK_CLOSE_PARAN) {
			printf("error: expected \")\" after <expr>\n");
			exit(1);
		}
		get_token(l);
		return new_node(N_FACTOR, left, NULL);
	}
	return NULL;
}

// ### terminal ###

node_t* id(lexer_t* l) {
	if(l->token.type == TK_ID) {
		get_token(l);
		return new_terminal(N_ID);
	}
	return NULL;
};
node_t* int_lit(lexer_t* l) {
	if(l->token.type == TK_INT_LIT) {
		get_token(l);
		return new_terminal(N_INT_LIT);
	}
	return NULL;
}

// ### main ###

node_t* get_syntax_tree(lexer_t* lexer)
{
	get_token(lexer);
	node_t* root = program(lexer);
	if(!root) {
		printf("error: expected top_elems in program\n");
		exit(1);
	}
	return root;
}

// ### display ###
int print_node(const node_t* node)
{
	switch (node->type) {
		case N_PROGRAM: printf("PROGRAM\n"); return 0;
		case N_TOP_ELEMS: printf("TOP_ELEMS\n"); return 0;
		case N_TOP_ELEM: printf("TOP_ELEM\n"); return 0;

		case N_FUNC_DEF: printf("FUNC_DEF\n"); return 0;
		case N_FUNC_SIG: printf("FUNC_SIG\n"); return 0;

		case N_COMP_STMT: printf("COMP_STMT\n"); return 0;
		case N_STMTS: printf("STMTS\n"); return 0;
		case N_STMT: printf("STMT\n"); return 0;

		case N_RETURN_STMT: printf("RETURN_STMT\n"); return 0;
		case N_DECL_STMT: printf("DECL_STMT\n"); return 0;
		case N_ASSIGN_STMT: printf("ASSIGN_STMT\n"); return 0;
		case N_IF_STMT: printf("IF_STMT\n"); return 0;
		case N_WHILE_STMT: printf("WHILE_STMT\n"); return 0;

		case N_EXPR: printf("EXPR\n"); return 0;
		case N_TERM: printf("TERM\n"); return 0;
		case N_FACTOR: printf("FACTOR\n"); return 0;

		case N_PLUS: printf("PLUS\n"); return 0;
		case N_MINUS: printf("MINUS\n"); return 0;
		case N_MULT: printf("MULT\n"); return 0;
		case N_DIV: printf("DIV\n"); return 0;

		case N_ID: printf("ID\n"); return 0;
		case N_INT_LIT: printf("INT_LIT\n"); return 0;

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
				printf("%lc     ",0x2502);
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
