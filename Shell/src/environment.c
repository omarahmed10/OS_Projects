#include "environment.h"


void setup_environment(void) {
	var_table = (lookup_table*) calloc(1000, sizeof(*var_table));
	set_variable("HOME", getenv("HOME"));
	set_variable("PATH", getenv("PATH"));
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	set_variable("CURR_PATH", cwd);
}
