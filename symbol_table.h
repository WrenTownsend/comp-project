#pragma once

typedef enum {
	S_UNKNOWN,
	S_CONST,
	S_VAR,
	S_FUNC,
}type_t;

typedef struct {
	int id;
	char name[32];
	type_t type;
}symbol_t;

#define TABLE_START_CAPACITY 5
#define TABLE_GROW_RATE 2
typedef struct {
	symbol_t* arr;
	int size;
	int capacity;
}table_t;

table_t* init_table();
int free_table(table_t*);
int create_entry(table_t*, char*); //returns id or returns -1 on error
