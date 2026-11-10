#pragma once

/* #include "config.h" */

typedef struct {
	char name[32];
	void* content; // TODO: temp void pointer because including config.h makes a loop
}entry_t;

#define TABLE_START_CAPACITY 5
#define TABLE_GROW_RATE 2
typedef struct {
	entry_t* arr;
	int size;
	int capacity;
}table_t;

table_t* init_table();
int free_table(table_t*);
int create_entry(table_t*, char*); //returns id or returns -1 on error
entry_t* get_entry(table_t*, char*);
int test_all_defined(table_t*);
