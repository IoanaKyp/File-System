#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define MAX_NAME_SIZE 10

struct Dir;
struct File;

typedef struct Dir {
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

/////////////////////// FUNCTII ADITIONALE \\\\\\\\\\\\\\\\\\\\\\\\

int searchFileInDir(Dir *parent, char *name);
int searchDirInDir(Dir *parent, char *name);
File *addFile(Dir *parent, char *name);
Dir *addDir (Dir *parent, char *name);
File *rmFile(Dir *parent, char *name);
Dir *rmThisDir(Dir *parent, char *name);
void deallocDir(Dir *target);
Dir *getLinkDirs(Dir *parent, char *name);
File *getLinkFiles(Dir *parent, char *name);
Dir *delOnlyDir(Dir *parent, char *name);

///////////////////// FUNCTII DE IMPLEMENTAT \\\\\\\\\\\\\\\\\\\\\\            

void touch (Dir *parent, char *name);
void mkdir (Dir *parent, char *name);
void ls (Dir *parent);
void rm (Dir *parent, char *name);
void rmdir (Dir *parent, char *name);
void cd(Dir **target, char *name);
char *pwd (Dir *target);
void stop (Dir *target);
void tree (Dir *target, int level);
void mv(Dir *parent, char *oldname, char *newname);



int main () {

	char *input = malloc(MAX_INPUT_LINE_SIZE);

	/* Creearea directorului radacina, home */
	Dir *home = malloc(sizeof(Dir));
	home->name = malloc(MAX_NAME_SIZE);
	strcpy(home->name, "home");
	home->head_children_dirs = NULL;
	home->head_children_files = NULL;
	home->next = NULL;
	home->parent = NULL;

	Dir *currentDir = home;

	/* String-uri folosite pentru citire + pwd */
	char *pwdPath = NULL;
	char *command = malloc(MAX_NAME_SIZE);
	char *param1 = malloc(MAX_NAME_SIZE);
	char *param2 = malloc(MAX_NAME_SIZE);

	do
	{
		fgets(input, MAX_INPUT_LINE_SIZE, stdin);
		
		if(strstr(input, "touch")) {
			sscanf(input, "%s %s\n", command, param1);
			touch(currentDir, param1);
		}
		if(strstr(input, "mkdir")) {
			sscanf(input, "%s %s\n", command, param1);
			mkdir(currentDir, param1);
		}
		if(strstr(input, "rm") && !strstr(input, "rmdir")) {
			sscanf(input, "%s %s\n", command, param1);
			rm(currentDir, param1);
		}
		if(strstr(input, "rmdir")) {
			sscanf(input, "%s %s\n", command, param1);
			rmdir(currentDir, param1);
		}
		if(strstr(input, "cd")) {
			sscanf(input, "%s %s\n", command, param1);
			cd(&currentDir, param1);
		}
		if(strstr(input, "tree")) {
			sscanf(input, "%s %s\n", command, param1);
			tree(currentDir, 0);
		}
		if(strstr(input, "pwd")) {
			pwdPath = pwd(currentDir);
			puts(pwdPath);
		}		
		if(strstr(input, "mv")) {
			sscanf(input, "%s %s %s\n", command, param1, param2);
			mv(currentDir, param1, param2);
		}		
		if(strstr(input, "ls")) {
			ls(currentDir);
		}		

	} while (strstr(input, "stop") == NULL);

	/* dezalocarea string-urilor folosite pentru citire/pwd */
	free(input);
	if(pwdPath != NULL) free(pwdPath);
	free(command); free(param1); free(param2);

	/* terminarea programului */
	stop(home);

	printf("Nu voi scrie niciodata, programul a fost oprit de stop :)");
	
	return 0;
}


/* Verifica daca un file se afla in directorul curent */
int searchFileInDir(Dir *parent, char *name) {
	//returneaza 1 daca exista, 0 daca nu
	if(parent->head_children_files == NULL) return 0;
	File *tmp = parent->head_children_files;
	if (strcmp(tmp->name, name) == 0) return 1;
	while(tmp->next != NULL) {
		tmp = tmp->next;
		if (strcmp(tmp->name, name) == 0) 
			return 1;
	}
	return 0;
}
/* Verifica daca un director se afla in directorul curent */
int searchDirInDir(Dir *parent, char *name) {
	//returneaza 1 daca exista, 0 daca nu
	if(parent->head_children_dirs == NULL) return 0;
	Dir *tmp = parent->head_children_dirs;
	if (strcmp(tmp->name, name) == 0) return 1;
	while(tmp->next != NULL) {
		tmp = tmp->next;
		if (strcmp(tmp->name, name) == 0) 
			return 1;
	}
	return 0;
}
/* Adaugarea unui nou file direct intr-un director */
File *addFile(Dir *parent, char *name) {
	//crearea unei noi entitati file
	File *newFile = malloc(sizeof(File));
	newFile->name = malloc(MAX_NAME_SIZE);
	strcpy(newFile->name, name);
	newFile->parent = parent;
	newFile->next = NULL;

	File *tmp = parent->head_children_files;
	File *files = parent->head_children_files;
	
	//inserarea noului in file la finalul lista parintelui
	if(tmp == NULL) {
		tmp = newFile;
		return tmp;
	} else {
		while(tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = newFile;
	}
	return files;
}

void touch (Dir *parent, char *name) {
	if(searchFileInDir(parent, name) == 1) {
		puts("File already exists");
	} else if(searchDirInDir(parent, name) == 1) {
		puts("File already exists");
	} else {
		parent->head_children_files = addFile(parent, name);
	}
}

/* Adaugarea unui nou director direct intr-un director */
Dir *addDir(Dir *parent, char *name) {
	//crearea unei noi entitati de tip dir
	Dir *newDir = malloc(sizeof(Dir));
	newDir->name = malloc(MAX_NAME_SIZE);
	strcpy(newDir->name, name);
	newDir->parent = parent;
	newDir->head_children_dirs = NULL;
	newDir->head_children_files = NULL;
	newDir->next = NULL;

	Dir *tmp = parent->head_children_dirs;
	Dir *dirs = parent->head_children_dirs;

	// inserarea noului director la finalul listei parintelui
	if(tmp == NULL){
		tmp = newDir;
		return tmp;
	} else {
		while(tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = newDir;
	}
	return dirs;
}

void mkdir (Dir *parent, char *name) {
	if(searchDirInDir(parent, name) == 1) {
		puts("Directory already exists");
	} else if(searchFileInDir(parent, name) == 1) {
		puts("Directory already exists");
	} else {
		parent->head_children_dirs = addDir(parent, name);
	}
}

void ls (Dir *parent) {
	//parcurgere si afisare directoare
	Dir *tmpDir = parent->head_children_dirs;
	while(tmpDir) {
		puts(tmpDir->name);
		tmpDir = tmpDir->next;
	}

	//parcurgere si afisare files
	File *tmpFile = parent->head_children_files;
	while(tmpFile) {
		puts(tmpFile->name);
		tmpFile = tmpFile->next;
	}
}

/* Stergerera directa a unui file dintr-un director */
File *rmFile(Dir *parent, char *name){
	File *tmp1, *tmp2, *copy;
	tmp1 = parent->head_children_files;
	tmp2 = parent->head_children_files;
	copy = parent->head_children_files;

	// daca este chiar primul file
	if(strcmp(tmp1->name, name) == 0) {
		tmp2 = tmp2->next;
		free(tmp1->name);
		free(tmp1);
		return tmp2;
	}
	//cautare file de sters in lista
	while(strcmp(tmp1->name, name)){
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	//modificarea legaturilor din lista si eliberarea
	//memoriei alocate file-ului eliminat
	tmp2->next = tmp1->next;
	free(tmp1->name);
	free(tmp1);
	return copy;
}

void rm (Dir *parent, char *name) {
	if(searchFileInDir(parent,name) == 0) {
		puts("Could not find the file");
	} else {
		parent->head_children_files = rmFile(parent, name);
	}
}

/* Dealocarea recursiva memoriei a memoriei unui director */
void deallocDir(Dir *target) {
	while(target->head_children_dirs != NULL) {
		deallocDir(target->head_children_dirs);	
	}
	while (target->head_children_files != NULL) {
		rm(target, target->head_children_files->name);
	}
	Dir *tmp = target->parent;
	if(tmp != NULL) {
		tmp->head_children_dirs = target->next;
	}
	free(target->name);
	free(target);
}
/* Stergerea directa a unui director dintr-un director */
Dir *rmThisDir(Dir *parent, char *name) {
	Dir* copy = parent->head_children_dirs;
	Dir* tmp1 = parent->head_children_dirs;
	Dir* tmp2 = parent->head_children_dirs;

	//daca este chiar primul director
	if(strcmp(tmp1->name, name) == 0) {
		tmp2 = tmp2->next;
		deallocDir(tmp1);
		return tmp2;
	}
	//cautare director de sters in lista
	while(strcmp(tmp1->name, name)) {
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	//modificarea legaturilor din lista si 
	//eliberarea memoriei alocate directorului eliminat
	tmp2->next = tmp1->next;
	deallocDir(tmp1);
	return copy;	
}

void rmdir (Dir *parent, char *name) {
	if(searchDirInDir(parent, name) == 0) {
		puts("Could not find the dir");
		return;
	}
	parent->head_children_dirs = rmThisDir(parent, name);
}

void cd(Dir **target, char *name) {
	//cazul urcarii in ierarhie
	if(strcmp(name, "..") == 0) {
		if((*target)->parent){
			*target = (*target)->parent;
		}
		return;
	}
	if(searchDirInDir(*target, name) == 0) {
		puts("No directories found!");
		return;
	}
	//cautarea noului director in lista parintelui
	Dir *tmp = (*target)->head_children_dirs;
	while(strcmp(tmp->name, name)) {
		tmp = tmp->next;
	}
	*target = tmp;
}

char *pwd (Dir *target) {
	char *path1 = malloc(MAX_INPUT_LINE_SIZE);
	char *path2 = malloc(MAX_INPUT_LINE_SIZE);
	Dir *tmp = target;
	while(tmp) {
		strcpy(path2,path1);
		strcpy(path1,"/");
		strcat(path1, tmp->name);
		strcat(path1, path2);
		tmp = tmp->parent;
	}
	free(path2);
	return path1;
}

void stop (Dir *target) {
	deallocDir(target);
	exit(0);
}

void tree (Dir *target, int level) {
	Dir *tmpD = target->head_children_dirs;
	File *tmpF = target->head_children_files;
	//afisarea directorelor din target
	while(tmpD != NULL) {
		//printarea taburilor
		int numberOfTabsD = level;
		while(numberOfTabsD) {
			printf("    ");
			numberOfTabsD--;
		}
		printf("%s\n", tmpD->name);

		// dupa ce am afisat un director intram in el
		tree(tmpD, level+1);

		tmpD = tmpD->next;
	}
	//afisarea file-urilor din target
	while(tmpF != NULL) {
		//printarea taburilor
		int numberOfTabsF = level;
		while(numberOfTabsF) {
			printf("    ");
			numberOfTabsF--;
		}
		printf("%s\n", tmpF->name);
		tmpF = tmpF->next;
	}
}

/* Returneaza un pointer la structura de dir a directorului "name" */
Dir *getLinkDirs(Dir *parent, char *name) {
	Dir *tmp = parent->head_children_dirs;
	while(strcmp(tmp->name, name) != 0) {
		tmp = tmp->next;
	}
	return tmp->head_children_dirs;
}
/* Returneaza un pointer la structura de file a directorului "name" */
File *getLinkFiles(Dir *parent, char *name) {
	Dir *tmp = parent->head_children_dirs;
	while(strcmp(tmp->name, name) != 0) {
		tmp = tmp->next;
	}
	return tmp->head_children_files;
}
/* Sterge un director fara a sterge ce se afla in el */
Dir *delOnlyDir(Dir* parent, char* name) {
	Dir* copy = parent->head_children_dirs;
	Dir* tmp1 = parent->head_children_dirs;
	Dir* tmp2 = parent->head_children_dirs;
	if(strcmp(tmp1->name, name) == 0) {
		tmp2 = tmp2->next;
		free(tmp1->name);
		free(tmp1);
		return tmp2;
	}
	while(strcmp(tmp1->name, name)) {
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	tmp2->next = tmp1->next;
	free(tmp1->name);
	free(tmp1);
	return copy;
}

void mv(Dir *parent, char *oldname, char *newname) {
	//daca nu exista obiectul de redenumit
	if(searchDirInDir(parent, oldname) == 0 && 
				searchFileInDir(parent, oldname) == 0) {

		puts("File/Director not found");
		return;
	}
	//daca numele cu care se doreste redenumirea exista deja
	if(searchDirInDir(parent, newname) == 1 || 
				searchFileInDir(parent, newname) == 1) {
		
		puts("File/Director already exists");
		return;
	}
	//redenumirea unui fisier
	if(searchFileInDir(parent, oldname)) {
		rm(parent, oldname);
		touch(parent, newname);
		return;
	}
	//redenumirea unui director
	if(searchDirInDir(parent, oldname)) {
		Dir *subDirHierarchy = getLinkDirs(parent, oldname);
		File *subFileHierarchy = getLinkFiles(parent, oldname);
		parent->head_children_dirs = delOnlyDir(parent, oldname);
		mkdir(parent, newname);
		Dir *tmp = parent->head_children_dirs;
		while(strcmp(tmp->name, newname) != 0) {
			tmp = tmp->next;
		}
		tmp->head_children_dirs = subDirHierarchy;
		tmp->head_children_files = subFileHierarchy;
		return;
	}	
}