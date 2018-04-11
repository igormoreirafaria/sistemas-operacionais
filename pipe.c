/* execill - How a parent and child might communicate. */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main(int argc, char ** argv) {
    int pid;
    int pc[2]; /* Parent to child pipe */
    int cp[2]; /* Child to parent pipe */
    char ch;
    int incount = 0, outcount = 0;

    /* Make pipes */
    if( pipe(pc) < 0)
    {
        perror("Can't make pipe");
        exit(1);
    }
    if( pipe(cp) < 0)
    {
        perror("Can't make pipe");
        exit(1);
    }


    /* Create a child to run command. */
    switch( pid = fork() )
    {
        case -1: 
            perror("Can't fork");
            exit(1);
        case 0:
            /* Child. */
            close(1); /* Close current stdout. */
            dup( cp[1]); /* Make stdout go to write
                            end of pipe. */
            close(0); /* Close current stdin. */
            dup( pc[0]); /* Make stdin come from read
                            end of pipe. */
            close( pc[1]);
            close( cp[0]);
            execvp(argv[1], argv + 1);
            perror("No exec");
            exit(1);
        default:
            /* Parent. */
            /* Close what we don't need. */
            printf("Input to child:\n");
            while(  read(0, &ch, 1) > 0 )
            {
                write(pc[1],&ch, 1);
                write(1, &ch, 1);
                incount ++;
            }
            close(pc[1]);
            printf("\nOutput from child:\n");
            close(cp[1]);
            while( read(cp[0], &ch, 1) == 1)
            {
                write(1, &ch, 1);
                outcount++;
            }
            printf("\n\nTotal characters in: %d\n",incount);
            printf("Total characters out: %d\n", outcount);
            exit(0);
    }

}

