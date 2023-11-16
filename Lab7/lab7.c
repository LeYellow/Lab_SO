#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    char litera[1];
    printf("introdu grupa:");
    scanf("%s",litera);
    execl("./stud","studenti.c", litera, NULL);
    printf("error exec");
    return 0;
}

/*
while "%c", litera;
sleep(500);

int pid = fork();
if (pid ==0)
{
    execl("./stud","studenti.c", litera, NULL);
    printf("error exec");
}
*/