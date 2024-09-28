#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>

// for insensitive compare between two strings
int stringCompare(char *string1 , char* string2, int insensitive)
{
    if(insensitive == 1){return strcasecmp(string1,string2) == 0;}
    else
    {
        return strcmp(string1,string2) == 0;
    }
}
// normal search not recursive
void searchFileInPath(char *searchPath, char *searchFile, int searchI)
{
    // The implementation of the searchFile function
    DIR *dir;
    struct dirent *file;
    int pathFound = 0;
    dir = opendir(searchPath);
    if(dir == NULL)
    {
        printf("Error opening directory \n");
    }
    else
    {
        while((file = readdir(dir))!= NULL)
        {
            if(file->d_type == DT_REG && stringCompare(file->d_name,searchFile,searchI) == 1)
            {
                pathFound = 1;
                break;
            }
        }
        pathFound != 0 ? printf("File Found at %s/%s\n",searchPath,searchFile) : printf("File is not Found at this Path\n");    
    }
    closedir(dir);
}


int main(int argc, char *argv[])
{
    int currOpt = 0;
    int searchR = 0; // check if R has been passed as an option
    int searchI = 0; // check if i has been passed as an option
    int numberOfArgumentPassed = 0; // Counter for the Argument passed
    while((currOpt = getopt(argc, argv, "Ri")) != EOF)
    {
        switch (currOpt)
        {
        case 'R':
            searchR = 1;
            numberOfArgumentPassed ++;
            break;
        case 'i':
            searchI = 1;
            numberOfArgumentPassed ++;
            break;
        case '?':
            printf("Invalid argument passed \n");
            exit(EXIT_FAILURE);
            break;
        }
    }
    // Check if the minimum number of Arguments is passed
    if(argc < numberOfArgumentPassed + 3)
    {
        printf("No file name or searchpath passed \n");
        exit(EXIT_FAILURE);
    }
    // Take the first argument as searchpath
    char *searchPath = argv[optind++];
    printf("The Searchpath passed is %s\n",searchPath);
    // Files to Search
    if(optind < argc)
    {
        for(int c = optind ; c < argc; c++)
        {
            // The fork implementation
            printf("The Argument passed is %s\n",argv[c]);
            searchFileInPath(searchPath,argv[c],searchI);
        }
    }
}
