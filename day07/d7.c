#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LINE 5000

typedef struct file_s {
	char name[50];
	int size;
} file_t;


typedef struct tree_node_s {
	char name[50];
	int cap_subdirectories;
	int nmb_subdirectories;
	struct tree_node_s** subdirectories;
	struct tree_node_s* parrent_dir;
	int nmb_files;
	int cap_files;
	file_t* files;
	int folder_size;
} tree_node_t;

typedef tree_node_t* tree_t;

tree_node_t* new_node(char* name, tree_node_t* parrent) {
	tree_node_t* rt = malloc(sizeof(tree_node_t));

	rt->parrent_dir = parrent;
	rt->folder_size = 0;
	rt->cap_files = 16;
	rt->nmb_files = 0;
	rt->cap_subdirectories = 16;
	rt->nmb_subdirectories = 0;
	rt->subdirectories = malloc(16*sizeof(tree_node_t*));
	rt->files = malloc(16*sizeof(file_t));
	assert(rt->subdirectories != NULL);
	assert(rt->files != NULL);

	strncpy(rt->name, name, 49);

	return rt;
}

tree_node_t* tree_add_subdir(tree_node_t* dir, char* name) {
	if(dir->cap_subdirectories > dir->nmb_subdirectories) {
		dir->subdirectories = realloc(dir->subdirectories, dir->cap_subdirectories *2);
		dir->cap_subdirectories *= 2;
		assert(dir->subdirectories != NULL);
	}
	dir->subdirectories[dir->nmb_subdirectories] = new_node(name, dir);
	dir->nmb_subdirectories++;

	return(dir->subdirectories[dir->nmb_subdirectories-1]);
}

void tree_add_file(tree_node_t* dir, int size, char* name) {
	if(dir->cap_files == dir->nmb_files) {
		dir->files = realloc(dir->files, dir->cap_files *2);
		dir->cap_files *= 2;
		assert(dir->files != NULL);
	}
	dir->files[dir->nmb_files].size = size;
	strncpy(dir->files[dir->nmb_files].name, name, 49);

	dir->nmb_files++;
}

tree_node_t* find_subdir(tree_node_t* dir, char* name) {
	for (int i = 0; i < dir->nmb_subdirectories; i++) {
		if(strncmp(dir->subdirectories[i]->name, name, 49) == 0) {
			return dir->subdirectories[i];
		}
		
	}
	return NULL; //if not found
}

bool find_file(tree_node_t* dir, char* name) {
	for (int i = 0; i < dir->nmb_subdirectories; i++) {
		if(strncmp(dir->files[i].name, name, 49) == 0) {
			return true;
		}
	}
	return false; //if not found
}

int tree_sum_rec(tree_t tree) {
	int fichers_somme = 0;

	for (int i = 0; i < tree->nmb_files; i++) {
		fichers_somme += tree->files[i].size;
	}

	int dir_somme = 0;
	for (int i = 0; i < tree->nmb_subdirectories; i++) {
		dir_somme += tree_sum_rec(tree->subdirectories[i]);
	}
	
	tree->folder_size = fichers_somme + dir_somme;
	return tree->folder_size;
}

void print_lvl(int lv) {
	for (int i = 0; i < lv; i++) {
		printf("|   ");
	}
	
}

void print_tree_rec(tree_t tree, int niveau) {
	print_lvl(niveau);
	printf("%s", tree->name);
	niveau++;

	for (int i = 0; i < tree->nmb_files; i++) {
		print_lvl(niveau);
		printf("%d %s", tree->files[i].size, tree->files[i].name);
	}
	for (int i = 0; i < tree->nmb_subdirectories; i++) {
		print_tree_rec(tree->subdirectories[i], niveau);
	}
}

int tree_sum_of_small_dirs(tree_t tree) {
	int somme = 0;

	if(tree->folder_size <= 100000) {
		somme += tree->folder_size;
	}

	int dir_somme = 0;
	for (int i = 0; i < tree->nmb_subdirectories; i++) {
		somme += tree_sum_of_small_dirs(tree->subdirectories[i]);
	}
	
	return somme;
}

int min(int a, int b) {
	return a < b ? a : b;
}

int tree_min_dir_to_dellete(tree_t tree, int space_to_free) {
	int minsize = 48729145;

	if(tree->folder_size >= space_to_free) {
		minsize = tree->folder_size;
	}

	int dir_somme = 0;
	for (int i = 0; i < tree->nmb_subdirectories; i++) {
		minsize = min(minsize, tree_min_dir_to_dellete(tree->subdirectories[i], space_to_free));
	}
	
	return minsize;
}

//88------------------------------------------------------------------------------
int main() {
	const char* file_name = "in.txt";
	FILE* file = fopen(file_name, "r");
	int line_count = 0;
	size_t linecapp = MAX_LINE;
	char* buff = (char*)malloc(sizeof(char) * linecapp);

	while (!feof(file)) {
		getline(&buff, &linecapp, file);
		line_count++;
	}
	fclose(file);
	printf("%d\n", line_count);

	char** lines = (char**)malloc(line_count * sizeof(char*));
	for (int i = 0; i < line_count; i++) {
		lines[i] = (char*)malloc(sizeof(char) * MAX_LINE);
	}
	
	file = fopen(file_name, "r");

	{int i = 0;
	while (!feof(file)) {
		getline(&lines[i], &linecapp, file);
		i++;
	}}

	// ########################### PROSSES DATA ###########################

	tree_t topdir = new_node("/\n", NULL);
	tree_node_t* courrant = topdir;

	for (int i = 0; i < line_count; i++)
	{
		//printf("line ok : %d\n", i);
		
		//printf("%d", i);
		if(lines[i][0] == '$') { //command
			if(strncmp(lines[i]+2, "cd", 2) == 0) {
				if(lines[i][5] == '/') {
					//printf("hallo");
					courrant = topdir;
				} else if(strncmp(lines[i]+5, "..", 2) ==0) {
					courrant = courrant->parrent_dir;
					if(courrant == NULL) {
						printf("line fail : %d\n", i);
						assert(false);
					}
				} else {
					char* dirname = lines[i]+5;
					tree_node_t* find = find_subdir(courrant, dirname);
					if (find == NULL) {
						courrant = tree_add_subdir(courrant, dirname);
					} else {
						courrant = find;
					}

				}
			} else if(strncmp(lines[i]+2, "ls", 2) == 0) {
				i++;
				while(lines[i][0] != '$' && lines[i][0] != 'S') {
					//printf("a");
					if (strncmp(lines[i], "dir", 3) == 0) {

						// dir
					} else if(isdigit(lines[i][0])) {
						//file
						int size = atoi(lines[i]);
						char* name = lines[i];
						while (isdigit(name[0]) || name[0] == ' ') {
							name++;
						}
						
						//find file
						//if not in add file
						if(!find_file(courrant, name))
							tree_add_file(courrant, size, name);
					} else {
						assert(false);
					}

					i++;
				}
				i--;
				if(lines[i][0] == 'S') {
					break;
				}
			} else {
				assert(false);
			}
		}
	}
	
	//printf("aggggggg");
	print_tree_rec(topdir, 0);

	printf("\n sum of all files %d\n", tree_sum_rec(topdir));
	printf("\n sum of small dirs %d\n", tree_sum_of_small_dirs(topdir));
	const int diskspace = 70000000;
	const int update_space = 30000000;
	const int space_used = topdir->folder_size;

	int space_to_free = update_space - (diskspace - space_used);
	
	printf("\n min dir to deleate size: %d\n", tree_min_dir_to_dellete(topdir, space_to_free));

	// ########################### PROSSES DATA ###########################

	for (int i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	free(lines);
}


