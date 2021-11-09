
struct Dir;
struct File;

typedef struct Dir{
	char *name; // The name of the directory
	struct Dir* parent; // Pointer to the parent directory
	struct File* head_children_files; // Pointer to the first child file
	struct Dir* head_children_dirs; // Pointer to the first child directory
	struct Dir* next; // Pointer to the next directory
} Dir;

typedef struct File {
	char *name; // The name of the file
	struct Dir* parent; // Pointer to the parent directory
	struct File* next; // Pointer to the next file
} File;

void touch (Dir* parent, char* name);

void mkdir (Dir* parent, char* name);

void ls (Dir* parent);

void rm (Dir* parent, char* name);

void rmdir (Dir* parent, char* name);

void cd(Dir** target, char *name);

char *pwd (Dir* target);

void stop (Dir* target);

void tree (Dir* target, int level);

void mv(Dir* parent, char *oldname, char *newname);


// Auxiliary functions
Dir *setUpDir();
File *setUpFile();
void freeDir(Dir **dir);