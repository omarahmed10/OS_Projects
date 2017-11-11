#include "commands.h"
/**
 * adding a new variable in the lookup table.
 */
void add_var(const char* var, const char* value);
/**
 * copying data from string (from) to string (to).
 */
void fill(char* to, int* to_indx, const char* from);

void cd(const char* path) {
	if (chdir(path) == -1) {
		printf("%s: No such file or directory\n", path);
	}
}

void echo(const char** messages) {
	size_t mess_size = 0;
	/*combining all messages sent in one message string.*/
	char* message = malloc(mess_size);
	for (int i = 0; messages[i] != NULL; i++) {
		message = realloc(message, strlen(messages[i]));
		//instead of strcat because of memory problems.
		fill(message, &mess_size, messages[i]);
	}
	message[mess_size] = '\0';
	printf("%s\n", message);
	free(message);
}
void fill(char* to, int* to_indx, const char* from) {
	for (int i = 0; from[i]; i++) {
		to[*to_indx] = from[i];
		(*to_indx)++;
	}
	to[*to_indx] = ' ';
	(*to_indx)++;
}
void export(const char** expressions) {
	int i = 0;
	/*exporting all variables declared.
	 * the expression is sent like that:
	 * i >> variable_name.
	 * i + 1 >> ='sign'
	 * i + 2 >> variable_value.
	 */
	while (expressions[i] != NULL) {
		add_var(expressions[i], expressions[i + 2]);
		i += 3;
	}
}
void add_var(const char* var, const char* value) {
	if (lookup_variable(var) != NULL) {
		edit_variable(var, value);
	} else {
		set_variable(var, value);
	}
}
