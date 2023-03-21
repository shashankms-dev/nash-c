#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TOKEN_BUFSIZE 64
#define TOKEN_DELIMITER " \t\r\n\a"

char *readline(void) {
	char *line = NULL;
	ssize_t bufsize = 0;
	
	if (getline(&line, &bufsize, stdin) == -1) {
		if (feof(stdin)) {
			exit(EXIT_SUCCESS);
		}
		else {
			perror("while reading line");
			exit(EXIT_FAILURE);
		}
	}

	return line;
}

char **tokenize(char *line) {
	int bufsize = TOKEN_BUFSIZE, count = 0;
	char **tokens = (char **) malloc(bufsize * sizeof(char*));
	char *token;

	if (tokens == NULL) {
		fprintf(stderr, "nash: Fatal memory allocation error \n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOKEN_DELIMITER);
	while(token != NULL) {
		tokens[count++] = token;

		if(count >= bufsize) {
			bufsize += TOKEN_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));

			if (tokens == NULL ) {
				fprintf(stderr, "nash: Fatal memory allocation error \n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOKEN_DELIMITER);
	}
	tokens[count] = NULL;
	return tokens;
}

int launch(char **args) {
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			perror("while executing program");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) {		// Error forking
		perror("while forking");
	}
	else {
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

void loop(void) {
	char prompt = '$';
	char *line;
	char **args;
	int status = 0;
	
	if (geteuid() == 0) {
		prompt = '#';
	}

	do {
		printf("%c ", prompt);
		line = readline();
		args = tokenize(line);
		status = execute(args);
		
		free(line);
		free(args);
    } while(status);
}
 
int main(int argc, char **argv)
{
	// Load config files, if any.
	
	// Run command loop.
	loop();
	
	// Perform any shutdown/cleanup.
	
	return EXIT_SUCCESS;
}
