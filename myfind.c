#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MIN_ARGS 0

void readArguments(int argc, char* argv[], char** searchPath, char*** files, int* R_opt, int* i_opt);
int searchFile(const char* path, char* name, int case_sensitive, int recursive);
int compare_filenames(const char* name, const char* file, int case_sensitive);
int files_length(char**files) ;

int main(const int argc, char* argv[])
{
    char* searchPath = NULL;
    char** files = NULL; // array of strings 
    int R_opt = 0, i_opt = 0;
    readArguments(argc, argv, &searchPath, &files, &R_opt, &i_opt);
    int files_len = files_length(files);
    pid_t pids[files_len];
    for(int c = 0; files[c] != NULL; c++ ) {
        pids[c] = fork();
        if(pids[c] == 0) {
            if(!searchFile(searchPath, files[c], i_opt,R_opt)) {
                printf("%d: %s not found%s in %s\n", getpid(), files[c], (R_opt ? " recursively" : ""), searchPath);
            };
            exit(EXIT_SUCCESS);
        }
         if(pids[c] < 0) exit(EXIT_FAILURE);
    }
    // Eltern warten bis kindprozess fertig ist um zombies zu vermeiden
    for (int c = 0; c < files_len; c++) waitpid(pids[c], NULL, 0);
    return 0;
}

int files_length(char**files) {
    int length = 0;
    while (files[length] != NULL) {
        length++;
    }
    return length;
}

void readArguments(const int argc, char*argv[], char** searchPath, char*** files,int *R_opt, int* i_opt) {
    int currOpt;
    while((currOpt = getopt(argc, argv, "Ri")) != EOF) {
        switch(currOpt) {
            case 'R':
                *R_opt = 1;
            break;
            case 'i':
                *i_opt = 1;
            break;
            case '?':
            default:
            exit(EXIT_FAILURE);
        }
    }
    const int numFiles = argc - optind;
    if(argc <= MIN_ARGS + 1 + *R_opt + *i_opt || numFiles <= 1) {
        printf("%s: Too few arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    *searchPath = argv[optind++];
    *files = malloc((numFiles + 1) * sizeof(char*));
    for(int file_count = 0; file_count < numFiles; file_count++) {
       (*files)[file_count] = argv[optind + file_count];
    }
    (*files)[numFiles] = NULL;
}

int searchFile(const char* path,  char* name, const int case_sensitive, const int recursive) {
    struct dirent *file;
    int found = 0;
    DIR* dr= opendir(path);
    if(dr == NULL) {
        printf("Could not open file!\n");
        exit(EXIT_FAILURE);
    }
    while ((file = readdir(dr)) != NULL) {
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) continue;

        char* fullPath = malloc(strlen(path) + strlen(file->d_name) + 2);
        sprintf(fullPath, "%s/%s", path, file->d_name);

        if (file->d_type == DT_REG) {
            if (compare_filenames(name, file->d_name, case_sensitive)) {
                printf("%d: %s: %s\n", getpid(), name, fullPath);
                found = 1;
            }
        }
        else if (file->d_type == DT_DIR && recursive) {
          if(searchFile(fullPath, name, case_sensitive, recursive)) found = 1;
        }
        free(fullPath);
    }
    closedir(dr);
    return found;
}

int compare_filenames(const char* name, const char* file, const int case_sensitive) {
    return (case_sensitive ? strcasecmp(name, file) == 0:strcmp(name, file) == 0);
}
