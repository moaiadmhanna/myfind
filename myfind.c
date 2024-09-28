#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>

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
    // printf("Number of Argument passed %i\n",argc);
    char *searchPath = argv[optind++];
    printf("The Searchpath passed is %s\n",searchPath);

    if(optind < argc)
    {
        for(int c = optind ; c < argc; c++)
        {
            // The fork implementation
            printf("The Argument passed is %s\n",argv[c]);
        }
    }
}

