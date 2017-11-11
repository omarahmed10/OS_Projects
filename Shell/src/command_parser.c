#include "command_parser.h"
/**
 * counting number of strings separated by spaces.
 */
int count_tokens(char* command);
/**
 * it takes a pointer to a string and return the first token in this string
 * and makes the pointer points on the first element after this token.
 */
char* string_token(char** command);
/**
 * it change the given command into array of string to create the argument list.
 */
char** split_command(char* command);

char** parse_command(const char* command) {
	char* command_cp = malloc(strlen(command) + 1);
	strcpy(command_cp, command);
	char** splited_command = split_command(command_cp);
	free(command_cp);
	return splited_command;
}

char** split_command(char* command) {
	int tokens_num = count_tokens(command);
	/*creating the array which will hold the argument list*/
	char **answer = calloc(tokens_num, 100);
	int i = 0;
	/*loop on the command to extract all tokens and it returns NULL.*/
	char* token = string_token(&command);
	while (token && strcmp(token, "")) {
		answer[i++] = strdup(token);
		token = string_token(&command);
	}
	answer[i] = NULL;
	return answer;
}
int count_tokens(char* command) {
	size_t count = 0;
	char* tmp = command;

	/* Count how many elements will be extracted. */
	while (*tmp) {
		if (*tmp++ == ' ') {
			count++;
		}
	}
	return count + 2;
}

char* string_token(char** command) {
	char *delimiter = " \t";
	char *sep = "\'\"=";
	size_t ans_size = 1;
	char *answer = malloc(ans_size);
	char *string = *command;
	bool is_message = false;
	while (*string) {
		/*first check if the current char is a space or tab*/
		if (strchr(delimiter, *string) == NULL) {
			/*
			 * check if the command have (single)double quotes and set a flag(is_message)=true
			 * when setting this flag it means that we will ignore any spaces(tabs) until next quote.
			 */
			if (strchr(sep, *string) != NULL) {
				is_message = true;
			}
			if (!is_message) {
				/*it is not a message then take all characters until the first space.*/
				int i = 0;
				while (strchr(delimiter, *string) == NULL
						&& strchr(sep, *string) == NULL
						&& *string) {
					*(answer + i++) = *string++;
					answer = realloc(answer, ++ans_size);
				}
				*(answer + i) = '\0';
			} else {
				//escaping double(single) quotes.
				int i = 0;
				if (*string != '=')
					string++;
				/*taking all the message between quotes.*/
				while (strchr(sep, *string) == NULL && *string) {
					*(answer + i++) = *string++;
					answer = realloc(answer, ++ans_size);
				}
				if (*string == '=')
					*(answer + i++) = *string++;
				*(answer + i) = '\0';
			}
			/*setting the new pointer of the command,
			 *  to make the command points on the first element after the last token.
			 */
			*command = string;
			return answer;
		}
		string++;
	}
	*command = string;
	return NULL;
}

