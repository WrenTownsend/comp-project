#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "symbol_table.h"

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
			exit(1);
			return 1;
			break;
	}
	return 0;
}
