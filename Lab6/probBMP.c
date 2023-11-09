#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#define BUFFER 8
#define PIC 3

int main(int argc, char *argv[])
{
    int fin,fout;
    char height[PIC],width[PIC];
    char buff[BUFFER],output[256];

    if(argc!=2)
    {
        perror("invalid number of arguments\n");
    }

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

    /*
    int=fs;
    struct stat fileInfo;
    fs=fstat(fin, &fileInfo);
    if (fs == -1) {
        perror("Error getting file information");
        close(fin);
        close(fout);
        exit(-1);
    }
    */

    //citire "poza"
    lseek(fin,18,SEEK_SET);
    read(fin,buff,BUFFER);

    printf("\nBuffer: %s\n\n",buff);

    strncpy(width,buff,3);
    strncpy(height,buff+4,3);

    //tiparire detalii in statistica.txt
    sprintf(output,"Inaltime : %s, Latime : %s",height, width);

    printf("Inaltime : %s, Latime : %s",height, width);

    if (write(fout,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    close(fin);
    close(fout);

    return 0;
}