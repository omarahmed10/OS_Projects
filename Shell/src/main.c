#include "main.h"

void start_shell(bool read_from_file, char *argv);
void shell_loop(bool input_from_file);
/**
 * Execute the command.
 */
int execute(const char* program, bool true_path,
		const char** arg_list, bool is_foreground);
/**
 * create the child process.
 */
void child_process(const char* program, const char** arg_list);
/**
 * try all paths in the PATH environment variable.
 */
char* get_program(const char* PATH, const char* program);
/**
 * check if the command will executed in back or fore ground.
 */
bool is_foreground(const char** arg_list);
/**
 * replacing variables and ~ with their proper values
 */
char* replace_delimiter(const char* message);
/**
 * decide which command will be executed.
 */
void start_execute(const char** arg_list);
/**
 * check whether the command in a comment or an empty command.
 */
bool check_comment(const char* command);
/**
 * writing logs.
 */
void write_to_log(int sig);

int main(int argc, char *argv[]) {

	setup_environment();
	load();
	open_log_file();
	/*Receiving child signal to write log.*/
	signal(SIGCHLD, write_to_log);
	if (argc > 1) {
		start_shell(true, argv[1]);
	} else {
		start_shell(false, argv[1]);
	}

	return 0;
}

void write_to_log(int sig) {
	//Reseting signal.
	signal(SIGCHLD, write_to_log);
	open_log_file();
	write_log("Child process was terminated\n");
	close_log_file();
}
void start_shell(bool read_from_file, char *argv) {
	cd(lookup_variable("HOME")); // let shell starts from home

	if (read_from_file) {
		open_commands_batch_file(argv);
		shell_loop(true);
	} else {
		shell_loop(false);
	}
}

void shell_loop(bool input_from_file) {
	bool from_file = input_from_file;

	while (true) {
		open_history_file();
		char* command = NULL;
		size_t command_size;
		if (from_file) {
			//read next instruction from file
			command = get_commands_batch_file();
			if (command == NULL) {
				close_commands_batch_file();
				from_file = false;
				continue;
			}
			if (!strcmp(command, "exit\n")) {
				//terminating shell..
				close_commands_batch_file();
				close_history_file();
				close_log_file();
				puts("terminating shell....");
				break;
			}
			if (strlen(command) > 512) {
				perror("too long command");
				continue;
			}
			printf("%s", command);
		} else {
			printf("Shell>");
			if (getline(&command, &command_size, stdin) == -1
					|| !strcmp(command, "exit\n")) {
				//terminating shell..
				puts("terminating shell....");
				break;
			}
			if (command_size > 512) {
				perror("too long command");
				continue;
			}
		}
		if (check_comment(command))
			continue;
		save_command(command);
		close_history_file();
		//removing the new line character in the command.
		command = strtok(command, "\n");
		strcat(command, "\0");
		command = replace_delimiter(command);
		const char** arg_list = (const char**) parse_command(
				command);
		if (arg_list == NULL) {
			continue;
		}
		start_execute(arg_list);
		free(arg_list);
	}
}
bool check_comment(const char* command) {
	for (int i = 0; command[i] != '\n'; i++) {
		if (*(command + i) != ' ' && *(command + i) == '#') {
			return true;
		} else if (*(command + i) != ' ') {
			return false;
		}
	}
	//empty command line.
	return true;
}
void start_execute(const char** arg_list) {
	if (!strcmp(arg_list[0], "cd")) {
		cd(arg_list[1]);
	} else if (!strcmp(arg_list[0], "echo")) {
		echo(arg_list + 1);
	} else if (!strcmp(arg_list[0], "history")) {
		print_history_file();
	} else if (!strcmp(arg_list[0], "export")) {
		export(arg_list + 1);
	} else if (arg_list[1] && strchr(arg_list[1], '=')) {
		export(arg_list);
	} else if (arg_list[0][0] == '/') {
		execute(arg_list[0], true, arg_list,
				is_foreground(arg_list));
	} else {
		execute(arg_list[0], false, arg_list,
				is_foreground(arg_list));
	}
}
char* replace_delimiter(const char* message) {
	const char *delimiter = "\'\" \t/\\";
	char *answer = malloc(512);
	int j = 0;
	for (int i = 0; i < strlen(message); i++) {
		if (message[i] == '$') {
			int k = 0;
			i++;
			size_t var_size = 1;
			/*taking the variable name which is right after the $sign.*/
			char* variable = malloc(var_size);
			while (strchr(delimiter, message[i]) == NULL
					&& i < strlen(message)) {
				variable[k++] = message[i++];
				variable = realloc(variable, ++var_size);
			}
			variable[k] = '\0';
			/*getting the variable value and substitute variable with its value it in the command.*/
			const char* value = lookup_variable(variable);
			if (value != NULL) {
				answer[j] = '\0';
				strcat(answer, value);
				j += strlen(value);
				--i;
			}
			free(variable);
		} else {
			if (message[i] == '~') {
				/*Substitute  ~sign with the current HOME environment variable*/
				const char* home = lookup_variable("HOME");
				answer[j] = '\0';
				strcat(answer, home);
				j += strlen(home);
			} else {
				answer[j++] = message[i];
			}
		}
	}
	answer[j] = '\0';
	return answer;
}
bool is_foreground(const char** arg_list) {
	int i = 0;
	while (arg_list[i] != NULL) {
		i++;
	}
	if (strchr(arg_list[i - 1], '&') != NULL) {
		arg_list[i - 1] = NULL;
		return false;
	} else {
		return true;
	}
}

int execute(const char* program, bool true_path,
		const char** arg_list, bool is_foreground) {
	siginfo_t childstat;
	pid_t child_pid;
	child_pid = fork();
	if (!child_pid) {
		if (true_path)
			execv(program, arg_list);
		else
			child_process(program, arg_list);
		fprintf(stderr, "%s: command not found\n", program);
		kill(getpid(), SIGQUIT);
	}
	if (is_foreground)
		waitid(P_PID, child_pid, &childstat, WEXITED);
	return child_pid;
}
void child_process(const char* program, const char** arg_list) {
	const char* PATH = lookup_variable("PATH");
	char* PATH_cp = malloc(strlen(PATH) + 1);
	strcpy(PATH_cp, PATH);
	char* token = strtok(PATH_cp, ":");
	while (token != NULL) {
		char *pro = get_program(token, program);
		execv(get_program(token, program), arg_list);
		free(pro);
		token = strtok(NULL, ":");
	}
}
char* get_program(const char* PATH, const char* program) {
	char* pro = malloc(strlen(program) + strlen(PATH) + 1);
	strcpy(pro, PATH);
	strcat(pro, "/");
	strcat(pro, program);
	return pro;
}
