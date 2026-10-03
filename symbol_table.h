typedef enum {
	T_CONST,
	T_VAR,
	T_FUNC,
}type_t;

typedef struct {
	char name[32];
	type_t type;
	int value;
}symbol_t;

typedef struct {
}table_t;

void create_entry();
