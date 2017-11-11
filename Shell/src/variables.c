#include "variables.h"

int next_var = 0;

const char* lookup_variable(const char* key) {
	for (int i = 0; i < next_var; i++) {
		if (strcmp(var_table[i].key, key) == 0)
			return var_table[i].value;
	}
	return getenv(key);
}

void set_variable(const char* key, const char* value) {
	var_table[next_var].key = malloc(strlen(key) + 1);
	strcpy(var_table[next_var].key, key);
	var_table[next_var].value = malloc(strlen(value) + 1);
	strcpy(var_table[next_var].value, value);
	next_var++;
}
void edit_variable(const char* key, const char* value) {
	for (int i = 0; i < next_var; i++) {
		if (strcmp(var_table[i].key, key) == 0)
			var_table[i].value = value;
	}
}
void print_all_variables(void) {
	printf("Printing Variables...\n");
	for (int i = 0; i < next_var; i++) {
		printf("%s >> %s\n", var_table[i].key,
				var_table[i].value);
	}
}
