#ifndef FILE_PROCESSING_H_   /* Include guard */
#define FILE_PROCESSING_H_

#include "main.h"

/* 
 history file basic functions' prototypes
 */
void open_history_file();
void close_history_file();
void save_command(char* command);
void print_history_file();
void load();
/* 
 log file basic functions' prototypes
 */
void open_log_file();
FILE* get_log_file();
void close_log_file();
void write_log(char* logs);

/* 
 CommandsBatch file basic functions' prototypes
 */
void open_commands_batch_file(const char* path);
char* get_commands_batch_file();
void close_commands_batch_file();

#endif // FILE_PROCESSING_H_
