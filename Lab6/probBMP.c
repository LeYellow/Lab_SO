#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int fin,fout;
    int height=0,width=0;
    char output[256];

    //verificare argumente
    if(argc!=2)
    {
        perror("invalid number of arguments\n");
    }

    //verificare fisiere
    fin=open(argv[1],O_RDONLY);
    if(fin==-1)
    {
        perror("invalid input file\n");
    }

    fout=open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC ,S_IRUSR | S_IWUSR);
    if(fout==-1)
    {
        perror("invalid output file\n");
    }

    //citire "poza"
    lseek(fin,18,SEEK_SET);
    read(fin, &width, sizeof(int));
    read(fin, &height, sizeof(int));

    //tiparire detalii in statistica.txt
    sprintf(output,"Inaltime : %d, Latime : %d",height, width);

    //printf("Inaltime : %d, Latime : %d",height, width);

    if (write(fout,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    close(fin);
    close(fout);

    return 0;
}
