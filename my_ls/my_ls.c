/*
 *  (c) Kyle O'Malley, 2022
 *
 *	A C program that prints a listing of all regular
 *	files in the current working directory, sorted
 *	in descending order of the file size. It should
 *	work in any directory without a fixed limit to
 *	the number of files it can list.
*/
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int cpmFiles(const void *_f1, const void *_f2)
{
	int ret = 0;
	char *f1 = *(char **)_f1;
	char *f2 = *(char **)_f2;

	struct stat *f1_stat = malloc(sizeof(struct stat));
	struct stat *f2_stat = malloc(sizeof(struct stat));

	stat(f1, f1_stat);
	stat(f2, f2_stat);

	// ret = f1_stat->st_size - f2_stat->st_size; //ascending
	ret = f2_stat->st_size - f1_stat->st_size; //descending

	free(f1_stat);
	free(f2_stat);

	return ret;
}

void printFiles(char ** filesList, int len){
	struct stat *buf = malloc(sizeof(struct stat));
	printf("\nFILES:\n");
	for(int i = 0; i < len; i++){
		stat(filesList[i], buf);
		printf("%s [%lld]\n", filesList[i], buf->st_size);
	}
	free(buf);
}

int main(int argc, char const *argv[])
{
	// varibables
	DIR *curDir;
	struct dirent *ent;
	int numFiles = 0;

	// open the current directory
	if ((curDir = opendir(".")) == NULL){
		printf("Failed to open directory.\n");
		return 0;
	}

	// count the number of files in the directory
	while ((ent = readdir(curDir)) != NULL)
	{
		if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) numFiles++;
	}
	rewinddir(curDir);

	// create a list of files
	char *filesList[numFiles];

	int i = 0;
	while ((ent = readdir(curDir)) != NULL)
	{
		if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
		{
			filesList[i] = malloc(strlen(ent->d_name) + 1);
			strcpy(filesList[i], ent->d_name);
			i++;
		}
	}

	qsort(filesList, numFiles, sizeof(char *), cpmFiles);

	printFiles(filesList, numFiles);

	for(i = 0; i < numFiles; i++) free(filesList[i]);
	closedir(curDir);
	return 0;
}