#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

lexer_t* lexer_init(char* file)
{
	lexer_t* lexer = malloc(sizeof(lexer_t));
	lexer->file = file;
	lexer->parse_point = file;
	lexer->token.type = TK_NULL;
	return lexer;
}

void lexer_reset(lexer_t* lexer)
{
	lexer->parse_point = lexer->file;
	lexer->token.type = TK_NULL;
	lexer->token.content[0] = '\0';
}

static int set_token(lexer_t* lexer, token_type token, char* start, char* end)
{
	
	lexer->parse_point = end;
	lexer->token.type = token;
	lexer->token.start = start; //inclusive
	lexer->token.end = end; //exclusive
	return 0;
}

static token_type lookup_keyword(char* str)
{
	// TODO: there is definatly a better way to do this maybe a hash table
	// then again this is c and that sounds like a pain to implemetn
	if(strcmp(str, "fn") == 0) return TK_FN;
	if(strcmp(str, "var") == 0) return TK_VAR;
	if(strcmp(str, "if") == 0) return TK_IF;
	if(strcmp(str, "while") == 0) return TK_WHILE;
	if(strcmp(str, "return") == 0) return TK_RETURN;
	return TK_ID;
}

int get_token(lexer_t* lexer)
{
	// clear content
	lexer->token.content[0] = '\0';

	char* p = lexer->parse_point;

	// skip whitespace and control chars
	while((isspace(*p) || iscntrl(*p)) && *p != '\0') {
		p++;
	}

	// skip comments
	if(p[0] == '/' && p[1] == '/') {
		p += 2;
		while(*p != '\n' && *p != '\0') {
			p++;
		}
		// skip whitespace and control chars
		while((isspace(*p) || iscntrl(*p)) && *p != '\0') {
			p++;
		}
	}

	switch(*p) {
		case ';':
			set_token(lexer, TK_SEMICOLON, p, p+1);
			break;
		case ',':
			set_token(lexer, TK_COMMA, p, p+1);
			break;
		case ':':
			set_token(lexer, TK_COLON, p, p+1);
			break;
		case '.':
			set_token(lexer, TK_DOT, p, p+1);
			break;
		case '(':
			set_token(lexer, TK_OPEN_PARAN, p, p+1);
			break;
		case ')':
			set_token(lexer, TK_CLOSE_PARAN, p, p+1);
			break;
		case '{':
			set_token(lexer, TK_OPEN_BRACE, p, p+1);
			break;
		case '}':
			set_token(lexer, TK_CLOSE_BRACE, p, p+1);
			break;
		case '[':
			set_token(lexer, TK_OPEN_BRACKET, p, p+1);
			break;
		case ']':
			set_token(lexer, TK_CLOSE_BRACKET, p, p+1);
			break;
		case '+':
			set_token(lexer, TK_PLUS, p, p+1);
			break;
		case '-':
			set_token(lexer, TK_MINUS, p, p+1);
			break;
		case '*':
			set_token(lexer, TK_MULT, p, p+1);
			break;
		case '/':
			set_token(lexer, TK_DIV, p, p+1);
			break;
		case '=':
			if(p[1] == '=') {set_token(lexer, TK_EQ, p, p+2); break;}
			set_token(lexer, TK_ASSIGN, p, p+1);
			break;
		case '>':
			set_token(lexer, TK_GT, p, p+1); break;
		case '<':
			set_token(lexer, TK_LT, p, p+1); break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{
				// TODO: could crash if writes past end of string
				int i = 0;
				for(;isdigit(p[i]); i++) {
					lexer->token.content[i] = p[i];
				}
				lexer->token.content[i] = '\0';
				set_token(lexer, TK_INT_LIT, p, p+i);
			}
			break;
		case '\0':
			set_token(lexer, TK_EOF, p, p+1);
			break;
		default:
			if(isalpha(*p)) {
				{
					// TODO: could crash if writes past end of string
					int i = 0;
					for(;isalnum(p[i]) || p[i] == '_'; i++) {
						lexer->token.content[i] = p[i];
					}
					lexer->token.content[i] = '\0';
					set_token(lexer, lookup_keyword(lexer->token.content), p, p+i);
				}
				break;
			}
			printf("error: couldn't find token (%c)\n", *p);
			return 1;
			break;
	}
	return 0;
}

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

