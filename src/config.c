#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "config.h"
#include "lexer.h"

element_c *make_element_nothing()
{
	element_c* new_element = calloc(1, sizeof(element_c));
	new_element->type = NOTHING;
	return new_element;
}
element_c *make_element_rule(rule_c* rule)
{
        element_c* new_element = calloc(1, sizeof(element_c));
	new_element->type = RULE;
	new_element->data.rule = rule;

	return new_element;
}
element_c *make_element_terminal(token_type token)
{
        element_c* new_element = calloc(1, sizeof(element_c));
	new_element->type = TERMINAL;
	new_element->data.terminal = token; 

	return new_element;
}
option_c *make_option(int num, ...)
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

rule_c *make_rule_list(int num, ...)
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
};

rule_c* example_ebnf()
{
	rule_c* example_rule = calloc(1, sizeof(rule_c));
        build_rule(example_rule, 2,
		make_option(2,
			make_element_terminal(TK_RETURN),
			make_element_terminal(TK_ASSIGN)),
		make_option(3,
			make_element_terminal(TK_FN),
			make_element_terminal(TK_FN),
			make_element_rule(example_rule)));

	/* meta_rule = make_rule( */
	/* 	make_option( */
	/* 		make_element("("), */
	/* 		make_element(other_rule), */
	/* 		make_element("EOF")), */
	/* 	make_option( */
	/* 		make_element(")") */
	/* 		make_element("EOF"))); */

	/* other_rule = make_rule( */
	/* 	make_option( */
	/* 		make_element("("))); */
	return example_rule;
};

