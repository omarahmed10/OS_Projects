/*
 * main.h
 *
 *  Created on: Oct 5, 2017
 *      Author: omar
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef enum {
	false = 0, true = 1
} bool;

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "command_parser.h"
#include "environment.h"
#include "file_processing.h"
#include "variables.h"
#include "commands.h"
#endif /* MAIN_H_ */
