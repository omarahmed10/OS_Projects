#include "file_processing.h"

/* 
 history file section
 */
FILE *history;
int history_counter;
void load() {
	open_history_file();
	char line[1024] = { 0, };
	while (fgets(line, 1024, history) != NULL) {
		// Just search for the latest line, do nothing in the loop
	}
	fclose(history);
	// This will look for Time and it will discard it collecting the number you are looking for in n2 and n3
	if (strlen(line) && sscanf(line, "  %d", &history_counter)) {
		history_counter++;
	} else {
		history_counter = 1;
	}
}
void open_history_file() {
	const char* cPath = lookup_variable("CURR_PATH");
	char history_path[strlen(cPath)];
	strcpy(history_path, cPath);
	strcat(history_path, "/history.txt");
	history = fopen(history_path, "a+");
}

void close_history_file() {
	fclose(history);
}

void save_command(char* command) {
	fprintf(history, "  %d  %s", history_counter++, command);
}

void print_history_file() {
	open_history_file();
	char line[1024] = { 0, };
	while (fgets(line, 1024, history) != NULL) {
		printf("%s", line);
	}
}
/* 
 log file section
 */
FILE* flog;
void open_log_file() {
	const char* lpath = lookup_variable("CURR_PATH");
	char log_path[strlen(lpath)];
	strcpy(log_path, lpath);
	strcat(log_path, "/log.txt");
	flog = fopen(log_path, "a+");
}

void write_log(char* logs) {
	fprintf(flog, "%s", logs);
}

void close_log_file() {
	fclose(flog);
}

/* 
 CommandsBatch file section
 */
FILE* batch;
void open_commands_batch_file(const char* path) {
	batch = fopen(path, "r");
	if (batch == NULL)
		printf("ERROR\n");
}

char* get_commands_batch_file() {
	char *line = malloc(1024);
	if (fgets(line, 1024, batch) != NULL) {
		return line;
	}
	return NULL;
}

void close_commands_batch_file() {
	fclose(batch);
}
