#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "lexer.h"
#include "debug.h"
#include "lang_parser.h"
#include "ebnf_parser.h"

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

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	init_color();

	if(argc < 3) {
		printf("all files not given\n");
		exit(1);
	} else if(argc > 3) {
		printf("too many files given\n");
		exit(1);
	}

	const char* ebnf_file_name = argv[1];
	const char* lang_file_name = argv[2];
	
	char* file = init_file(ebnf_file_name);
	if(file == NULL) {
		printf("	at init_file\n");
		return 1;
	}
	char* lang_file = init_file(lang_file_name);
	if(file == NULL) {
		printf("	at init_file\n");
		return 1;
	}

	printf("### EBNF FILE ###\n");
	{
		char current_char = 1;
		char* read = file;
		while(current_char != '\0') {
			printf("%c", *read);
			read++;
			current_char = *read;
		}
	}

	//init table
	table_t* table = init_table();

	printf("\n### TOKENS ###\n");
	lexer_t* lexer = lexer_init(file, table);
	while(lexer->token.type != TK_EOF) {
		if(next_token(lexer)) {
			printf("	at next_token\n");
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

	printf("\n### PARSE EBNF ###\n");
	parse_ebnf(lexer, table);
	printf("parsed ebnf file\n");

	//set entry point
	rule_c* entry_rule = get_entry(table, "main")->content;

	printf("\n### LANGUAGE FILE ###\n");

	{
		char current_char = 1;
		char* read = lang_file;
		while(current_char != '\0') {
			printf("%c", *read);
			read++;
			current_char = *read;
		}
	}


	printf("\n### CHECK SYNTAX ###\n");
        if (check_syntax(lexer, entry_rule)) {
		printf("syntax is correct!!!\n");
	}

	/* printf("\n### SYNTAX TREE ###\n"); */
	/* print_tree(tree); */

	printf("\n### TABLE TEST ###\n");
	print_table(table);
	// cleanup
	free(table->arr);
	free(table);

	// TODO: also free the tree and table
	free(lexer);
	lexer = NULL;
	return 0;
}
