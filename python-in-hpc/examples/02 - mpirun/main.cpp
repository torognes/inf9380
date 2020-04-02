#include <stdio.h>
#include <string.h>

// Simple program to dump the contents of the command line parameters from mpirun
int main(int argc, char **argv, char **envp)
{
    printf("Command line parameters passed to the program (%d)\n", argc);

    for(int i=0; i<argc; i++)
        printf("argc[%d] - \"%s\"\n", i, argv[i]);

    while(*envp != NULL)
    {
        if(
            strncasecmp(*envp, "OMPI_", 5) == 0 || 
            strncasecmp(*envp, "PMIX_", 5) == 0 || 
            strncasecmp(*envp, "OPAL_", 5) == 0 || 
            strncasecmp(*envp, "PMIX_", 5) == 0
            )
        printf("%s\n", *envp);
        envp++;
    }

    return 0;
}
