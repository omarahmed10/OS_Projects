#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_
#include "main.h"
/* 
 - This function should be responsible for implementing the "cd" shell command
 */
void cd(const char* path);

/* 
 - This function should be responsible for implementing the "echo" shell command
 - Typically should lookup any variable included in the message
 */
void echo(const char** messages);

void export(const char** expressions);

#endif // COMMANDS_H_
