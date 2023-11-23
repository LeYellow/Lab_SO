//parintele citeste 1 char si trimite copilului prin pipe, copilu citeste si afiseaza

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

int pfd[2];
int pid;

void parent()
{
    close(pfd[0]);
    char caracter[2];
    printf("Baga caracter: ");
    scanf("%s",caracter);
    write(pfd[1],&caracter,sizeof(caracter));
    close(pfd[1]);
}

void son()
{
    char result[2];
    close(pfd[1]);
    read(pfd[0],&result,sizeof(result));
    printf("Uite caracter: %s\n",result);
    close(pfd[0]);
}

int main()
{
    //char charcater[2];
    //printf("Baga caracter: ");
    //scanf("%s",charcater);

	if(pipe(pfd)<0)
	{
	    perror("Eroare la crearea pipe-ului\n");
	    exit(1);
	}
	
	if((pid=fork())<0)
	{
	    perror("Eroare la fork\n");
	    exit(1);
	}

	if(pid==0) 
	{
    
        /* procesul fiu*/ 

	    //close(pfd[0]); /* inchide capatul de citire; *//* procesul va scrie in pipe  */

	    //write(pfd[1],charcater,strlen(charcater)); /* operatie de scriere in pipe */   

	    //close(pfd[1]); /* la sfarsit inchide si capatul utilizat */
    	//exit(0);
        parent();
        exit(0);
	}
	/* procesul parinte */	
	//close(pfd[1]); /* inchide capatul de scriere; *//* procesul va citi din pipe  */
    son();
    //char citire[2];
	//read(pfd[0],citire,1); /* operatie de citire din pipe */
	//printf(" %s \n",citire);

	//close(pfd[0]); /* la sfarsit inchide si capatul utilizat */
	
	return 0;
}