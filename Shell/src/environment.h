#ifndef ENVIRONMENT_H_   /* Include guard */
#define ENVIRONMENT_H_

#include "main.h"

typedef struct {
	char* key;
	char* value;
} lookup_table;

lookup_table *var_table;
/* 
 - This function should be responsible for importing environment variables into your project.
 - Typically, this function should add $PATH, $HOME & any other needed variables into your variables table
 - Any future work on variables added by this function should be done through your variable table

 - You also need to store the path of the directory containing this project into a variable,
 you'll need it in printing the history & log into a files beside your executable file
 */
void setup_environment(void);

#endif // ENVIRONMENT_H_
