#include <stdio.h>
#include <stdlib.h>
#include "ebnf_parser.h"
#include "lexer.h"

int check_element(lexer_t *l, table_t* t, element_c *element);
int check_option(lexer_t *l, table_t* t, option_c *option);
int check_rule(lexer_t *l, table_t* t, rule_c* rule);

int check_element(lexer_t *l, table_t* t, element_c *element)
{
	if (element->type == TERMINAL && element->data.terminal == l->token.type) {
		next_token(l);
		return 1;
	}
        if (element->type == RULE && check_rule(l, t, t->arr[element->data.rule].content)) {
		return 1;
	}
        if (element->type == NOTHING) {
		return 1;
	}
	return 0;
}

int check_option(lexer_t *l, table_t* t, option_c *option)
{
	if (!check_element(l, t, option->element_list)) {
		return 0;
	}
	for (element_c *current_element = option->element_list->next; current_element; current_element = current_element->next) {
		if (!check_element(l, t, current_element)) {
			printf("error: expected something after something\n");
			exit(1);
		}
	}
	return 1;
}

int check_rule(lexer_t *l, table_t* t, rule_c* rule)
{
	for (option_c* current_option = rule->option_list; current_option; current_option = current_option->next){ // for every option
                if(check_option(l, t, current_option)) {
			return 1;	
		}
	}
	return 0;
}

int check_syntax(lexer_t* lexer, table_t* table, rule_c* rule)
{
	next_token(lexer);

        if (check_rule(lexer, table, rule)) {
		return 1;
	}
	return 0;
};
