#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "lexer.h"
#include "debug.h"
#include "ebnf_parser.h"
#include "config.h"

char* init_file(const char* file_name)
{
	FILE* file = fopen(file_name, "r");
	if(!file) {
		printf("error: failed to open file\n");
		return NULL;
	}

	long length;
	char* buffer;

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	rewind(file);

	buffer = calloc(1, length + 1);
	if(!buffer) {
		printf("error: failed to allocate memory\n");
		return NULL;
	}

	fread(buffer, length, 1, file);

	fclose(file);

	return buffer;
}

int main()
{
	setlocale(LC_ALL, "");
	init_color();

	const char* file_name = "tests/ebnf_test.lang";
	char* file = init_file(file_name);
	if(file == NULL) {
		printf("	at init_file\n");
		return 1;
	}

	printf("### FILE ###\n");
	char current_char = 1;
	char* read = file;
	while(current_char != '\0') {
		printf("%c", *read);
		read++;
		current_char = *read;
	}

	//init table
	table_t* table = init_table();

	printf("\n### TOKENS ###\n");
	lexer_t* lexer = lexer_init(file, table);
	while(lexer->token.type != TK_EOF) {
		if(get_token(lexer)) {
			printf("	at get_token\n");
			return 1;
		}
		if(print_token(&lexer->token)) {
			printf("	at print_token\n");
			return 1;
		}
	}

	// cleanup
	lexer_reset(lexer);
	free(table->arr);
	free(table);
	table = init_table();

	printf("\n### LOAD EBNF ###\n");
	rule_c* rules = example_ebnf();

	printf("\n### CHECK SYNTAX ###\n");
        if (check_syntax_tree(lexer, rules)) {
		printf("syntax is correct!!!\n");
	}
	/* printf("\n### SYNTAX TREE ###\n"); */
	/* print_tree(tree); */

	printf("\n### TABLE TEST ###\n");
	print_table(table);

	// TODO: also free the tree and table
	free(lexer);
	lexer = NULL;
	return 0;
}
