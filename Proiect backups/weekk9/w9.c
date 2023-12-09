#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>

int pipeEnds[2];
int pipe_id;

/*
void funct(char *litera, char *file)
{
    execl("./shell.sh", "shell.sh", litera, file,NULL);
    printf("erroare execl");
}
*/

int main(int argc, char **argv)
{
    char *litera=argv[1];
    int stat;
    //int sta;

    int pipeEnds[2];
    if(pipe(pipeEnds)<0)
    {
        printf("Error pipe");
        exit(1);
    }
    int pipeFork;
    if((pipeFork=fork())<0)
    {
        printf("fork error");
        exit(1);
    }
    else if (pipeFork==0)
    {
        close(pipeEnds[0]);
        if(dup2(pipeEnds[1],STDOUT_FILENO)<0)
        {
            printf("Redirect error");
            exit(1);
        }

        //execl("./shell.sh","./shell.sh", litera, NULL);
        execlp("bash","bash","shell.sh",litera,(char*)NULL);
        printf("Executie error");
        exit(1);
    }
    //printf("S-a incheiat procesul de grayscale cu pid: %d si codul: %d\n", wait(&sta), sta);

    close(pipeEnds[1]);
    char counter[16];
    if(read(pipeEnds[0], counter, sizeof(counter))<0)
    {
        printf("Preluare pipe error");
        exit(1);
    }

    int result=atoi(counter);
    printf("%d",result);

    printf("S-au gasit %d prop cu %s",WEXITSTATUS(stat),litera);

	return 0;
}