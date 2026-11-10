#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rule_table.h"

table_t* init_table()
{
	table_t* table = calloc(1, sizeof(table_t));
	table->arr = malloc(sizeof(entry_t) * TABLE_START_CAPACITY);
	table->size = 0;
	table->capacity = TABLE_START_CAPACITY;
	return table;
}

int free_table(table_t* table)
{
}

int grow_table(table_t* table)
{
	table->arr = realloc(table->arr, \
		      sizeof(entry_t) * table->capacity * TABLE_GROW_RATE);
	table->capacity *= TABLE_GROW_RATE;
	return 0;
}

int create_entry(table_t* table, char* name)
{
	//check if entry already exists
	for(int i = 0; i < table->size; i++){
		if(strcmp(table->arr[i].name, name) == 0) { // if strings are the same
			return i; //return id
		}
	}

	//grow arr if its to small
	// TODO: should check if resize was succesfull or throw an error
	if((table->size + 1) > table->capacity) {
		grow_table(table);
	}

	//assine values
	strcpy(table->arr[table->size].name, name);

	//inc size
	table->size++;

	//return id
	return table->size - 1;
}

entry_t* get_entry(table_t* table, char* name)
{
	for(int i = 0; i < table->size; i++){
		if(strcmp(table->arr[i].name, name) == 0) { // if strings are the same
			return &table->arr[i];
		}
	}
	printf("error: couldn't find \"%s\" in table\n", name);
	exit(1);
}

int test_all_defined(table_t* table)
{
	for(int i = 0; i < table->size; i++){
		if(!table->arr[i].content) {
			printf("warning: %s not defined\n", table->arr[i].name);
			/* exit(1); */
		}
	}
}
