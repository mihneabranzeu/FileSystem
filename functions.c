#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tema1.h"


Dir *setUpDir() {
	// Allocate memory for the directory
	Dir *dir = (Dir *)malloc(sizeof(Dir));
	if (dir == NULL) {
		printf("Memory allocation for directory failed");
		return NULL;
	}

	// Set up its properties
    dir->name = NULL;
	dir->parent = NULL;
	dir->head_children_dirs = NULL;
	dir->head_children_files = NULL;
	dir->next = NULL;

    return dir;
}

File *setUpFile() {
	// Allocate memory for the file
	File *file = (File *)malloc(sizeof(File));
	if (file == NULL) {
		printf("Memory allocation for file failed");
		return NULL;
	}

	// Set up its properties
	file->name = NULL;
    file->next = NULL;
    file->parent = NULL;

    return file;
}

void destroyFile(File *file) {
    if (!file) {
        printf("File has not been initialised\n");
        return;
    }
    // Free the file
    free(file->name);
    free(file);
}

void destroyDir(Dir *dir) {
    if (!dir) {
        printf("Directory has not been initialised\n");
        return;
    }
    // Free the directory
    free(dir->name);
    free(dir);
}

void includeFileInSystem(File *file, Dir *parent) {
    // Check if file is null
    if (file == NULL) {
        printf("File has not been initialised");
        return;
    }

    File *curr = parent->head_children_files;
    if (curr == NULL) {
        // This is the first file in the directory
        parent->head_children_files = file;
        return;
    }

    // Go through the list of files
    while (curr->next != NULL)
    {
        // Check if the file alredy exists
        if (strcmp(curr->name, file->name) == 0) {
            printf("File already exists\n");
            // Destroy the file
            destroyFile(file);
            return;
        }
        curr = curr->next;
    }

    // Check if the file alredy exists
    if (strcmp(curr->name, file->name) == 0) {
        printf("File already exists\n");
        // Destroy the file
        destroyFile(file);
        return;
    }

    // Add the file into the list
    curr->next = file;   
}

void includeDirectoryInSystem(Dir *dir, Dir *parent) {
    // Check if directory is null
    if (dir == NULL) {
        printf("Directory has not been initialised\n");
        return;
    }

    Dir *curr = parent->head_children_dirs;
    if (curr == NULL) {
        // This is the first directory in the parent directory
        parent->head_children_dirs = dir;
        return;
    }

    // Go through the list of directories
    while (curr->next != NULL) {
        // Check if the directory already exists
        if (strcmp(curr->name, dir->name) == 0) {
            printf("Directory already exists\n");
            destroyDir(dir);
            return;
        }
        curr = curr->next;
    }

    // Check if the directory already exists
    if (strcmp(curr->name, dir->name) == 0) {
        printf("Directory already exists\n");
        destroyDir(dir);
        return;
    }

    // Add the directory into the list
    curr->next = dir;
}

void freeFileList(File **file) {
    // Check if file has been allocated
    if (file == NULL || *file == NULL) {
        return;
    }

    // Go through the list of files and free them
    File *tmpFile;
    while ((*file) != NULL)
    {
        tmpFile = *file;
        *file = (*file)->next;
        free(tmpFile->name);
        free(tmpFile);
    }

    // Free the file itself
    free(*file);
    file = NULL;
}

void freeDir(Dir **dir) {
    // Check if dir has been allocated
    if (dir == NULL || *dir == NULL) {
        return;
    }

    Dir *tmpDir;

    // Go through the list of dirs and free them and their contents
    while ((*dir)->head_children_dirs != NULL) {
        tmpDir = (*dir)->head_children_dirs;
        (*dir)->head_children_dirs = (*dir)->head_children_dirs->next;
        freeFileList(&(tmpDir->head_children_files));
        freeDir(&(tmpDir->head_children_dirs));
        free(tmpDir->name);
        free(tmpDir);
    }

    freeFileList(&(*dir)->head_children_files);

    free((*dir)->name);
    free(*dir);
    dir = NULL;
}



void touch (Dir* parent, char* name) {
    // Create the file
    File *newFile = setUpFile();
    if (!newFile) {
        return;
    }

    // Initialize its properties
    newFile->name = name;
    newFile->parent = parent;

    // Include the file in the system
    includeFileInSystem(newFile, parent);
}

void ls (Dir *parent) {
    // Check if the directory has not been initialised
    if (!parent) {
        printf("The directory has not been initialised");
        return;
    }

    // Print the children directories
    Dir *currDir = parent->head_children_dirs;
    while (currDir != NULL) {
        printf("%s\n", currDir->name);
        currDir = currDir->next;
    }

    // Print the children files
    File *currFile = parent->head_children_files;
    while (currFile) {
        printf("%s\n", currFile->name);
        currFile = currFile->next;
    }
}

void mkdir(Dir *parent, char *name) {
    if (!parent) {
        printf("The parent directory has not been initialised");
        return;
    }

    // Create the new directory
    Dir *newDir = setUpDir();
    if (!newDir) {
        return;
    }
    // Initialise its properties
    newDir->name = name;
    newDir->parent = parent;

    // Include the directory in the system
    includeDirectoryInSystem(newDir, parent);
}

void rm(Dir *parent, char *name) {
    // Check if parent has been initialised
    if (!parent) {
        printf("The parent directory has not been initialised\n");
        return;
    }

    // Check if the file list is empty
    if (parent->head_children_files == NULL) {
        printf("Could not find the file\n");
        return;
    }
    
    File *curr = parent->head_children_files;
    File *prev = parent->head_children_files;
    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0) {
            // Check if the node is the head of the list
            if (curr == parent->head_children_files) {
                parent->head_children_files = parent->head_children_files->next;
                destroyFile(curr);
            } else {
                prev->next = curr->next;
                destroyFile(curr);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Could not find the file\n");
}

void rmdir(Dir *parent, char *name) {
    // Check if parent has been initialised
    if (!parent) {
        printf("The parent directory has not been initialised");
        return;
    }

    // Check if the list is empty
    if (parent->head_children_dirs == NULL) {
        printf("Could not find the dir\n");
        return;
    }

    Dir *curr = parent->head_children_dirs;
    Dir *prev = parent->head_children_dirs;

    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            // Check if the node is the head of the list
            if (curr == parent->head_children_dirs) {
                parent->head_children_dirs = parent->head_children_dirs->next;
                
                freeDir(&curr);
            } else {
                prev->next = curr->next;
                freeDir(&curr);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Could not find the dir\n");
}

void cd(Dir **target, char *name) {
    // Check if the directory has been initialised
    if (!(*target) || !target) {
        printf("Target directory has not been initialised\n");
        return;
    }

    // Check for special command ..
    if (strcmp(name, "..") == 0) {

        if ((*target)->parent != NULL) {
            *target = (*target)->parent;
        }
        return;
    }

    Dir *curr = (*target)->head_children_dirs;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            *target = curr;
            return;
        }
        curr = curr->next;
    }
    printf("No directories found!\n");
}

void stop(Dir *target) {
    freeDir(&target);
}

char* _pwd(Dir *target, char **s) {
    if (target->parent != NULL) {
        _pwd(target->parent, s);
    }
    if (*s == NULL) {
        *s = malloc(strlen(target->name) + 2);
        strcpy(*s, "/");
        strcat(*s, target->name);
    } else {
        char *newString = realloc(*s, strlen(*s) + strlen(target->name) + 2);
        *s = newString;
        strcat(*s, "/");
        strcat(*s, target->name);
    }
    return *s;
}

char* pwd(Dir *target) {
    char *s = NULL;
    return _pwd(target, &s);
}

void tree(Dir *target, int level) {
    if(target == NULL) {
        
        return;
    }
    Dir *currDir = target->head_children_dirs;
    while (currDir != NULL) {
        for (int i = 0; i < level; i++) {
            printf("    ");
        }
        
        printf("%s\n", currDir->name);
        tree(currDir, level + 1);
        
        File *file = currDir->head_children_files;
        while (file != NULL) {
            for (int i = 0; i <= level; i++) {
                printf("    ");
            }
            printf("%s\n", file->name);
            file  = file->next;
        }
        currDir = currDir->next;
    }
    if (level == 0) {
        File *file = target->head_children_files;
        while (file != NULL) {
            for (int i = 0; i < level; i++) {
                printf("    ");
            }
            printf("%s\n", file->name);
            file  = file->next;
        }
    }
    
    
}

void mv (Dir *parent, char *oldName, char *newName) {
    File *currFile = parent->head_children_files;

    while (currFile != NULL) {
        if (strcmp(currFile->name, newName) == 0) {
            printf("File/Director already exists\n");
            return;
        }
        currFile = currFile->next;
    }

    currFile = parent->head_children_files;
    Dir *curr = parent->head_children_dirs;

    while (curr != NULL) {
        if (strcmp(curr->name, newName) == 0) {
            printf("File/Director already exists\n");
            return;
        }
        curr = curr->next;
    }
    curr = parent->head_children_dirs;



    while (currFile != NULL) {
        if (strcmp(oldName, currFile->name) == 0) {
            rm(parent, currFile->name);
            touch(parent, strdup(newName));
            return;
        }
        currFile = currFile->next;
    }
    
    Dir *prev = parent->head_children_dirs;
    
    while (curr != NULL) {
        if (strcmp(curr->name, oldName) == 0) {
            // Check if the node is the head of the list
            free(curr->name);
            curr->name = strdup(newName);
            if (curr == parent->head_children_dirs) {
                parent->head_children_dirs = parent->head_children_dirs->next;
                curr->next = NULL;
                includeDirectoryInSystem(curr, parent);
            } else {
                printf("altul");
                prev->next = curr->next;
                curr->next = NULL;
                includeDirectoryInSystem(curr, parent);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    
    printf("File/Director not found\n");
}