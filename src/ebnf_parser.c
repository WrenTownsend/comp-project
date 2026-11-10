#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "ebnf_parser.h"
#include "lexer.h"

rule_c* new_rule(lexer_t* l, table_t* t)
{
	rule_c* new_rule = calloc(1, sizeof(rule_c));
	if(l->token.type != TK_ID) {
		printf("error: expected ID for rule\n");
		exit(1);
	}
	get_entry(t, l->token.content)->content = new_rule; //set table entry pointer
	next_token(l);
	return new_rule;
}
option_c* new_option(lexer_t* l)
{
	option_c* new_option = calloc(1, sizeof(option_c));
	return new_option;
}
element_c* new_element(lexer_t* l, table_t* t)
{
	element_c* new_element = calloc(1, sizeof(element_c));

	if(l->token.type == TK_ID) {
		new_element->type = RULE;
		new_element->data.rule = (rule_c**)&get_entry(t, l->token.content)->content;
		next_token(l);
		return new_element;
	}
	if(l->token.type == TK_QUOTE) {
		next_token(l);
		if(l->token.type == TK_QUOTE) { //empty string
			next_token(l);
			new_element->type = NOTHING;
			return new_element;
		}
		//terminal
		new_element->type = TERMINAL;
		new_element->data.terminal = l->token.type;
		next_token(l);
		if(l->token.type != TK_QUOTE) {
			printf("error: expecter \" after element"); // TODO: make this more spacific
			exit(1);
		}
		next_token(l);
		return new_element;

	}
	
	printf("error: expected rule or teminal for element\n");
	exit(1);
}

int parse_ebnf(lexer_t* l, table_t* t) // t is to keep track of variable names
{
	next_token(l);

	rule_c* current_rule; //eat id and put it on t
	option_c* current_option;
	element_c* current_element; //eat id and return element of correct type

	// have t with rule names and a pointer to pass to add_element
	
	current_rule = new_rule(l, t);
	for(;;) { // rules loop
		if(l->token.type != TK_ASSIGN) {
			printf("error: expected \"=\" after rule id\n");
			exit(1);
		}
		next_token(l);

		current_option = current_rule->option_list = new_option(l);
		for(;;) { // options loop

			current_element = current_option->element_list = new_element(l,t);
			for(;;) { // elements loop
				if(l->token.type == TK_OR || l->token.type == TK_SEMICOLON) {
					break;
				}
				if(l->token.type != TK_COMMA) {
					printf("error: expected \",\" after element\n");
					exit(1);
				}
				next_token(l);
				
				current_element = current_element->next = new_element(l, t);
			}
			if(l->token.type == TK_SEMICOLON) {
				break;
			}
			if(l->token.type != TK_OR) {
				printf("error: expected \"|\" or \";\" after option\n");
				exit(1);
			}
			next_token(l);

			current_option = current_option->next = new_option(l);
		}
		if(l->token.type != TK_SEMICOLON) {
			printf("error: expected \";\" after rule\n");
			exit(1);
		}
		next_token(l);

		if(l->token.type == TK_EOF) {
			break;
		}

		current_rule = new_rule(l, t);
	}

	test_all_defined(t);
	return 0;
}
