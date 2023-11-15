#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define OUT_SIZE 256

struct stat fileInfo;

int argCheck(int x)
{
    if(x!=2)
    {
        perror("invalid number of arguments\n");
        return 1;
    }
    return 0;
}

int inputFileCheck(char *filename)
{
    int fin=open(filename,O_RDONLY);
    if(fin==-1)
    {
        perror("invalid input file\n");
    }
    return fin;
}

int checkIfBMP(char *name)
{
    char *ext = strchr(name,'.');
    if(strcmp(ext,".bmp")!=0)
    {
        perror("file is not bmp");
        return 1;
    }
    return 0;
}

int getFileStat(int fileIN)
{
    int stat=fstat(fileIN, &fileInfo);
    if(stat==-1)
    {
        perror("error getting file info");
    }
    return stat;
}

int outputFileCreator()
{
    int fout=open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC ,S_IRUSR | S_IWUSR);
    if(fout==-1)
    {
        perror("invalid output file\n");
    }
    return fout;
}

void outputWriter(int fileIN,int fileOUT, int fileSTATS, char *name)
{
    char output[OUT_SIZE];
    int height=0,width=0;

    lseek(fileIN,18,SEEK_SET);
    read(fileIN, &width, sizeof(int));
    read(fileIN, &height, sizeof(int));
    sprintf(output,"File name: %s \nInaltime: %dpx \nLatime: %dpx\n", name, height, width);

    if (write(fileOUT,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }
}

int main(int argc, char *argv[])
{
    if(argCheck(argc))
    {
        exit(-1);
    }

    int fin=inputFileCheck(argv[1]);
    int fs=getFileStat(fin);

    if(checkIfBMP(argv[1]))
    {
        exit(-1);
    }

    int fout=outputFileCreator();
    outputWriter(fin,fout,fs,argv[1]);

    close(fin);
    close(fout);

    return 0;
}