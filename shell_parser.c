#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INPUT_LENGTH 	2048
#define MAX_ARGS		512

char *HOME_PATH = "/home";


struct command_line
{
	char *argv[MAX_ARGS + 1];
	int argc;
	char *input_file;
	char *output_file;
	bool is_bg;
};


struct command_line *parse_input()
{
	char input[INPUT_LENGTH];
	struct command_line *curr_command = (struct command_line *) calloc(1, sizeof(struct command_line));

	// Get input
	printf(": ");
	fflush(stdout);
	fgets(input, INPUT_LENGTH, stdin);

	// Ignore comments
    if(input[0] == '#'){
		return curr_command;
	}
	// Tokenize the input
	char *token = strtok(input, " \n");
	while(token){
		if(!strcmp(token,"<")){
			curr_command->input_file = strdup(strtok(NULL," \n"));
		} else if(!strcmp(token,">")){
			curr_command->output_file = strdup(strtok(NULL," \n"));
		} else if(!strcmp(token,"&") && *token+1 == 0){
			curr_command->is_bg = true;
		} else{
			curr_command->argv[curr_command->argc++] = strdup(token);
		}
		token=strtok(NULL," \n");
	}

	return curr_command;
}

void change_dir(struct command_line *curr_command){
	char *path = (char *) calloc(2048, sizeof(char *));
	if(curr_command->argc == 1){
		strcpy(path, HOME_PATH);
	} else {
		strcpy(path, curr_command->argv[1]);
	}
	chdir(path);
	getcwd(path, 2048);
	setenv("PWD", path, 1);
	free(path);
}

void execute_command(struct command_line *curr_command){
	char *command_name = (char *) calloc(32, sizeof(char *));
	strcpy(command_name, curr_command->argv[0]);
	// Execute a built-in command or default to another
	// Change directory
	if(!strcmp(command_name, "cd")){
		change_dir(curr_command);
	}
	// Free space
	free(command_name);
}

int main()
{
	struct command_line *curr_command;

	while(true)
	{
		curr_command = parse_input();

		if(curr_command->argc > 0){
			execute_command(curr_command);
		}
	}
	return EXIT_SUCCESS;
}