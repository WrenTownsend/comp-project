#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "config.h"
#include "lexer.h"

element_c* make_element_nothing()
{
	element_c* new_element = calloc(1, sizeof(element_c));
	new_element->type = NOTHING;
	return new_element;
}
element_c* make_element_rule(rule_c* rule)
{
        element_c* new_element = calloc(1, sizeof(element_c));
	new_element->type = RULE;
	new_element->data.rule = rule;
	return new_element;
}
element_c* make_element_terminal(token_type token)
{
        element_c* new_element = calloc(1, sizeof(element_c));
	new_element->type = TERMINAL;
	new_element->data.terminal = token; 
	return new_element;
}
option_c* make_option(int num, ...)
{
	va_list list;
	va_start(list, num);

	option_c* new_option = calloc(1, sizeof(option_c));
	
	element_c* current_element = va_arg(list, element_c*); // TODO: throw error
	new_option->element_list = current_element;
     
	for(int i = 0; i < num - 1; i++) {
		current_element->next = va_arg(list, element_c*);
		current_element = current_element->next;
	}

	va_end(list);
	return new_option;
} // this is litteraly the exact some code with diffrent names im an awful programer

void build_rule(rule_c* rule, int num, ...)
{
	va_list list;
	va_start(list, num);

	option_c* current_option = va_arg(list, option_c*); // TODO: throw error
	rule->option_list = current_option;
     
	for(int i = 0; i < num - 1; i++) {
		current_option->next = va_arg(list, option_c*);
		current_option = current_option->next;
	}

	va_end(list);
	return;
}

rule_c* make_rule_list(int num, ...)
{
	va_list list;
	va_start(list, num);

	rule_c* new_rule = calloc(1, sizeof(rule_c));
	
	option_c* current_option = va_arg(list, option_c*); // TODO: throw error
	new_rule->option_list = current_option;
     
	for(int i = 0; i < num - 1; i++) {
		current_option->next = va_arg(list, option_c*);
		current_option = current_option->next;
	}

	va_end(list);
	return new_rule;
}

rule_c* example_ebnf()
{
	rule_c* example_rule = calloc(1, sizeof(rule_c));
        build_rule(example_rule, 2,
		make_option(1,
			make_element_terminal(TK_SEMICOLON)),
		make_option(3,
			make_element_terminal(TK_FN),
			make_element_terminal(TK_FN),
			make_element_rule(example_rule)));

	return example_rule;
}

rule_c* new_rule(lexer_t* l)
{
	rule_c* new_rule = calloc(1, sizeof(rule_c));
	if(l->token.type != TK_ID) {
		printf("error: expected ID for rule\n");
		exit(1);
	}
	get_token(l);
	return new_rule;
}
option_c* new_option(lexer_t* l)
{
	option_c* new_option = calloc(1, sizeof(option_c));
	return new_option;
}
element_c* new_element(lexer_t* l)
{
	element_c* new_element = calloc(1, sizeof(element_c));
	// TODO: set type of element and table stuff
	// just skips the element right now
	if(l->token.type == TK_QUOTE) { // TODO: VERRRRY termorary
		get_token(l);
		get_token(l);
	}
	get_token(l);
	
	new_element->type = TERMINAL;
	return new_element;
}

rule_c* parse_ebnf_file(lexer_t* l, table_t* table) // table is to keep track of variable names
{
	get_token(l);

	rule_c* root_rule;

	rule_c* current_rule; //eat id and put it on table
	option_c* current_option;
	element_c* current_element; //eat id and return element of correct type

	// have table with rule names and a pointer to pass to add_element
	
	current_rule = root_rule = new_rule(l);
	for(;;) { // rules loop
		if(l->token.type != TK_ASSIGN) {
			printf("error: expected \"=\" after rule id\n");
			exit(1);
		}
		get_token(l);

		current_option = current_rule->option_list = new_option(l);
		for(;;) { // options loop

			current_element = current_option->element_list = new_element(l);
			for(;;) { // elements loop
				if(l->token.type == TK_OR || l->token.type == TK_SEMICOLON) {
					break;
				}
				if(l->token.type != TK_COMMA) {
					printf("error: expected \",\" after element\n");
					exit(1);
				}
				get_token(l);
				
				current_element = current_element->next = new_element(l);
			}
			if(l->token.type == TK_SEMICOLON) {
				break;
			}
			if(l->token.type != TK_OR) {
				printf("error: expected \"|\" or \";\" after option\n");
				exit(1);
			}
			get_token(l);

			 current_option = current_option->next = new_option(l);
		}
		if(l->token.type != TK_SEMICOLON) {
			printf("error: expected \";\" after rule\n");
			exit(1);
		}
		get_token(l);

		if(l->token.type == TK_EOF) {
			break;
		}

		current_rule = current_rule->next = new_rule(l);
	}
	return root_rule;
}
