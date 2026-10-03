#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "lexer.h"
#include "parser.h"

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

	const char* file_name = "test/test.lang";
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

	printf("\n### TOKENS ###\n");
	lexer_t* lexer = lexer_init(file);
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

	printf("\n### SYNTAX DEBUG ###\n");
	node_t* tree = get_syntax_tree(lexer);

	printf("\n### SYNTAX TREE ###\n");
	print_tree(tree);

	free(lexer);
	lexer = NULL;
	return 0;
}
