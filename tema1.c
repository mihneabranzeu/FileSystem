#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tema1.h"

#define MAX_INPUT_LINE_SIZE 300

int main () {
	// Set up the variabless that will manage the input reading
	char *line = NULL;
	size_t len = 0;
	ssize_t lineSize = 0;

	// Create the home directory
	Dir *homeDir = setUpDir();
	homeDir->name = strdup("home");
	
	// Keep track of the current directory
	Dir *currDir = homeDir;
	do
	{
		// Read and format the command 
		lineSize = getline(&line, &len, stdin);
		line[lineSize - 1] = '\0';
		lineSize--;

		if (strstr(line, "touch")) {
			touch(currDir, strdup(line + strlen("touch ")));
		} else if (strcmp(line, "ls") == 0) {
			ls(currDir);
		} else if (strcmp(line, "stop") == 0) {
			// Free the memory for the system;
			stop(homeDir);
			free(line);
			line = NULL;
			break;
		} else if (strstr(line, "mkdir")) {
			mkdir(currDir, strdup(line + strlen("mkdir ")));
		} else if (strstr(line, "rmdir")) {
			rmdir(currDir, line + strlen("rmdir "));
		} else if (strstr(line, "rm")) {
			rm(currDir, line + strlen("rm "));
		} else if (strstr(line, "cd")) {
			cd(&currDir, line + strlen("cd "));
		} else if (strcmp(line, "pwd") == 0) {
			char *path = pwd(currDir);
			printf("%s\n", path);
			free(path);
		} else if (strcmp(line, "tree") == 0) {
			tree(currDir, 0);
		} else if (strstr(line, "mv")) {
			char *names = line + strlen("mv ");
			int posSpace = 0;
			for (int i = 0; i < strlen(names); i++) {
				if (names[i] == ' ') {
					posSpace = i;
					break;
				}
			}
			char *oldName = malloc(posSpace + 1);
			strncpy(oldName, names, posSpace);
			oldName[posSpace] = '\0';
			char *newName = names + posSpace  + 1;
			mv(currDir, oldName, newName);
			free(oldName);
		}
		free(line);
		line = NULL;
	} while (1);

	return 0;
}
