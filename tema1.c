#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Dir {
	char *name;   // numele directorului
	struct Dir* parent;  // pointer catre parintele directorului(null pentru radacina)
	struct File* head_children_files;  // pointer catre primul element de tip File din interiorul directorului
	struct Dir* head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir* next; // pointer catre urmatorul element din lista in care se afla directorul
}Dir; // structura de tip director
 
typedef struct File {
	char *name;  // numele fisierului
	struct Dir* parent; // pointer catre directorul pe
	struct File* next; // pointer catre urmatorul element din lista de fisiere
}File; // structura de tip fisier

Dir* init_dir(Dir* parent, char name[])
{
	Dir* newDir = malloc(sizeof(*newDir));
	newDir->name = malloc(strlen(name) + 1);
	memcpy(newDir->name, name, strlen(name) + 1);
	newDir->head_children_dirs = NULL;
	newDir->head_children_files = NULL;
	newDir->next = NULL;
	newDir->parent = parent;
	return newDir;
}

File* init_file(Dir* parent, char name[])
{
	File* newFile = malloc(sizeof(*newFile));
	newFile->name = malloc(strlen(name) + 1);
	memcpy(newFile->name, name, strlen(name) + 1);
	newFile->next = NULL;
	newFile->parent = parent;
	return newFile;
}

void free_file(File* file)
{
	free(file->name);
	free(file);
}

void wipe_files(File* file)
{
	if (!file)
		return;
	if (file->next)
		wipe_files(file->next);
	free_file(file);
}

void free_dir(Dir* directory)
{
	if (!directory)
		return;
	Dir* q = directory->head_children_dirs;
	while (q)
	{
		Dir* p = q;
		q = q->next;
		free_dir(p);
	}
	wipe_files(directory->head_children_files);
	free(directory->name);
	free(directory);
}

void touch (Dir* parent, char* name)
{
	if (parent->head_children_files == NULL)
		parent->head_children_files = init_file(parent, name);
	else
	{
		File* q = parent->head_children_files;
		if (strcmp(q->name, name) == 0)
		{
			printf("File already exists\n");
			return;
		}
		while (q->next != NULL)
		{
			if (strcmp(q->name, name) == 0)
			{
				printf("File already exists\n");
				return;
			}
			q = q->next;
		}
		q->next = init_file(parent, name);
	}
}

void mkdir (Dir* parent, char* name)
{
	if (parent->head_children_dirs == NULL)
		parent->head_children_dirs = init_dir(parent, name);
	else
	{
		Dir* q = parent->head_children_dirs;
		if(strcmp(q->name, name) == 0)
		{
			printf("Directory already exists\n");
			return;
		}
		while (q->next != NULL)
		{
			if(strcmp(q->name, name) == 0)
			{
				printf("Directory already exists\n");
				return;
			}
			q = q->next;
		}
		q->next = init_dir(parent, name);
	}
}

void ls (Dir* parent)
{
	if (!parent)
		return;
	for (Dir* iter = parent->head_children_dirs; iter != NULL; iter = iter->next)
		printf("%s\n", iter->name);
	for (File* iter = parent->head_children_files; iter != NULL; iter = iter->next)
		printf("%s\n", iter->name);
}

void rm (Dir* parent, char* name)
{
	File* q = parent->head_children_files;
	if (q == NULL)
	{
		printf("Could not find the file\n");
		return;
	}
	else if (strcmp(q->name, name) == 0)
	{
		parent->head_children_files = parent->head_children_files->next;
		free_file(q);
		return;
	}
	else
		for (File* iter = q; iter->next != NULL; iter = iter->next)
		{
			if (strcmp(iter->next->name, name) == 0)
			{
				File* p = iter->next;
				iter->next = iter->next->next;
				free_file(p);
				return;
			}
		}
	printf("Could not find the file\n");
}

void rmdir (Dir* parent, char* name)
{
	Dir* q = parent->head_children_dirs;
	if (q == NULL)
	{
		printf("Could not find the dir\n");
		return;
	}
	else if (strcmp(q->name, name) == 0)
	{
		parent->head_children_dirs = parent->head_children_dirs->next;
		free_dir(q);
		return;
	}
	else
		for (Dir* iter = q; iter->next != NULL; iter = iter->next)
			if (strcmp(q->next->name, name) == 0)
			{
				Dir* p = iter->next;
				iter->next = iter->next->next;
				free_dir(p);
				return;
			}
	printf("Could not find the dir\n");
}

void cd (Dir** target, char* name)
{
	if (strcmp(name, "..") == 0)
	{
		if ((*target)->parent != NULL)
			*target = (*target)->parent;
		return;
	}
	else
		for (Dir* iter = (*target)->head_children_dirs; iter != NULL; iter = iter->next)
			if (strcmp(iter->name, name) == 0)
			{
				*target = iter;
				return;
			}
	printf("No directories found!\n");
}

void tree (Dir* target, int level)
{
	for (Dir* iter = target->head_children_dirs; iter != NULL; iter = iter->next)
	{
		for (int i = 0; i < level; i++)
			printf("    ");
		printf("%s\n", iter->name);
		tree(iter, level + 1);
	}
	for (File* iter = target->head_children_files; iter != NULL; iter = iter->next)
	{
		for (int i = 0; i < level; i++)
			printf("    ");
		printf("%s\n", iter->name);
	}
}

void reconst_path(Dir* target, char* path)
{
	if (target->parent->parent)
		reconst_path(target->parent, path);
	strcat(path, "/");
	strcat(path, target->name);
}

char* pwd (Dir* target)
{
	char* path = malloc(300);
	strcpy(path, "/home");
	Dir* q = target;
	if (q->parent == NULL)
		return path;
	reconst_path(q, path);
	return path;
}

void stop(Dir* target)
{
	free_dir(target);
}

void mv(Dir* parent, char* oldname, char* newname)
{
	Dir* q = parent->head_children_dirs;
	File* p = parent->head_children_files;
	int found = 0;
	while (q && found == 0)
	{
		if (strcmp(q->name, oldname) == 0)
			found = 1;
		q = q->next;
	}
	while (p && found == 0)
	{
		if (strcmp(p->name, oldname) == 0)
			found = 1;
		p = p->next;
	}
	if (found == 0)
	{
		printf("File/Director not found\n");
		return;
	}
	q = parent->head_children_dirs;
	p = parent->head_children_files;
	found = 0;
	while (q && found == 0)
	{
		if (strcmp(q->name, newname) == 0)
			found = 1;
		q = q->next;
	}
	while (p && found == 0)
	{
		if (strcmp(p->name, newname) == 0)
			found = 1;
		p = p->next;
	}
	if (found == 1)
	{
		printf("File/Director already exists\n");
		return;
	}
	q = parent->head_children_dirs;
	p = parent->head_children_files;
	if (parent->head_children_dirs != NULL)
	{
		while(q->next)
			q = q->next;
		if (strcmp(parent->head_children_dirs->name, oldname) == 0)
		{
			Dir* head = parent->head_children_dirs;
			parent->head_children_dirs->name = realloc(parent->head_children_dirs->name, strlen(newname) + 1);
			memcpy(parent->head_children_dirs->name, newname, strlen(newname) + 1);
			if (head->next)
			{
				parent->head_children_dirs = parent->head_children_dirs->next;
				q->next = head;
				head->next = NULL;
			}
			return;
		}
		else
			for (Dir* iter = parent->head_children_dirs; iter->next != NULL; iter = iter->next)
				if (strcmp(iter->next->name, oldname) == 0)
				{
					q->next = iter->next;
					iter->next = iter->next->next;
					q->next->next = NULL;
					parent->head_children_dirs->name = realloc(parent->head_children_dirs->name, strlen(newname) + 1);
					memcpy(parent->head_children_dirs->name, newname, strlen(newname) + 1);
					return;
				}
	}
	if (parent->head_children_files != NULL)
	{
		while(p->next)
			p = p->next;
		if (strcmp(parent->head_children_files->name, oldname) == 0)
		{
			File* head = parent->head_children_files;
			parent->head_children_files->name = realloc(parent->head_children_files->name, strlen(newname) + 1);
			memcpy(parent->head_children_files->name, newname, strlen(newname) + 1);
			if (head)
			{
				parent->head_children_files = parent->head_children_files->next;
				p->next = head;
				head->next = NULL;
			}
			return;
		}
		else
		{
			for (File* iter = parent->head_children_files; iter->next != NULL; iter = iter->next)
				if (strcmp(iter->next->name, oldname) == 0)
				{
					p->next = iter->next;
					iter->next = iter->next->next;
					p->next->next = NULL;
					parent->head_children_files->name = realloc(parent->head_children_files->name, strlen(newname) + 1);
					memcpy(parent->head_children_files->name, newname, strlen(newname) + 1);
					return;
				}
			printf("File/Director not found\n");
			return;
		}
	}
	else
		printf("File/Director not found\n");
}

void get_command(char *command[], int *argC)
{
	char *input = malloc(300);
	fgets(input, 300, stdin);
	*argC = 0;
	char *argV = strtok(input, " ");
	while (argV)
	{
		int argLen = strlen(argV);
		if (argV[argLen - 1] == '\n')
		{
			argV[argLen - 1] = '\0';
			argLen--;
		}
		memcpy(command[*argC], argV, argLen + 1);
		*argC += 1;
		argV = strtok(NULL, " ");
	}
	free(input);
}

int process_command(char *command[], Dir** target)
{
	if (strcmp(command[0], "touch") == 0)
	{
		touch(*target, command[1]);
		return 1;
	}
	else if (strcmp(command[0], "mkdir") == 0)
	{
		mkdir(*target, command[1]);
		return 2;
	}
	else if (strcmp(command[0], "ls") == 0)
	{
		ls(*target);
		return 3;
	}
	else if (strcmp(command[0], "rmdir") == 0)
	{
		rmdir(*target, command[1]);
		return 5;
	}
	else if (strcmp(command[0], "rm") == 0)
	{
		rm(*target, command[1]);
		return 4;
	}
	else if (strcmp(command[0], "cd") == 0)
	{
		cd(target, command[1]);
		return 6;
	}
	else if (strcmp(command[0], "tree") == 0)
	{
		tree(*target, 0);
		return 7;
	}
	else if (strcmp(command[0], "pwd") == 0)
	{
		char* path;
		path = pwd(*target);
		printf("%s\n", path);
		free(path);
		return 8;
	}
	else if (strcmp(command[0], "stop") == 0)
	{
		while((*target)->parent)
			*target = (*target)->parent;
		stop(*target);
		return 0;
	}
	else if (strcmp(command[0], "mv") == 0)
	{
		mv(*target, command[1], command[2]);
		return 9;
	}
	else
	{
		return -1;
	}
}

int main(void)
{
	int i, argC = 0, signal = 0;
    char *command[4];
	Dir* root = init_dir(NULL, "home");
	for (i = 0; i < 4; i++)
		command[i] = malloc(50);
	do
	{
		get_command(command, &argC);
		signal = process_command(command, &root);
	}while(signal != 0);
	for (i = 0; i < 4; i++)
		free(command[i]);
	return 0;
}
