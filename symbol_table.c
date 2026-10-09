#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

table_t* init_table()
{
	table_t* table = calloc(1, sizeof(table_t));
	table->arr = malloc(sizeof(symbol_t) * TABLE_START_CAPACITY);
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
		      sizeof(symbol_t) * table->capacity * TABLE_GROW_RATE);
	table->capacity *= TABLE_GROW_RATE;
	return 0;
}

int create_entry(table_t* table, char* name)
{
	//grow arr if its to small
	// TODO: should check if resize was succesfull or throw an error
	if((table->size + 1) > table->capacity) {
		grow_table(table);
	}

	//assine values
	table->arr[table->size].id = table->size;
	strcpy(table->arr[table->size].name, name);
	table->arr[table->size].type = S_UNKNOWN; // TODO: this is temporary

	//inc size
	table->size++;

	//return id
	return table->size - 1;
}
