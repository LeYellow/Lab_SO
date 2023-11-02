#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#define BUFFER 1

int main(int argc, char *argv[])
{
    int fin,fout,fs;
    int uppercaseCount=0,lowercaseCount=0,digitCount=0,charCount=0;
    char buff[BUFFER],output[256],ch = argv[3][0];
    struct stat fileInfo;

    if(argc!=4)
    {
        perror("invalid number of arguments\n");
    }

    fin=open(argv[1],O_RDONLY);
    if(fin==-1)
    {
        perror("invalid input file\n");
    }

    fout=open(argv[2],O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR);
    if(fout==-1)
    {
        perror("invalid input file\n");
    }

    fs=fstat(fin, &fileInfo);
    if (fs == -1) {
        perror("Error getting file information");
        close(fin);
        close(fout);
        exit(-1);
    }

    while(read(fin,buff,BUFFER)>0)
    {
        if(islower(buff[0]))
        {
            lowercaseCount++;
        } 
        else if(isupper(buff[0]))
        {
            uppercaseCount++;
        } 
        else if(isdigit(buff[0]))
        {
            digitCount++;
        }
        if(ch==buff[0])
        {
            charCount++;
        }
    }

    sprintf(output,"Numar litere mici: %d\nNumar litere mari: %d\nNumar cifre: %d\nNumar aparitii caracter: %d\nDimensiune fisier: %ld\n",
lowercaseCount,uppercaseCount,digitCount,charCount,fileInfo.st_size);

    if (write(fout,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    close(fin);
    close(fout);

    return 0;
}