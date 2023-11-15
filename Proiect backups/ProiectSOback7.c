#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#define OUT_SIZE 512

struct stat fileInfo;
struct dirent *dirInfo;

int argCheck(int x)
{
    if(x!=2)
    {
        perror("invalid number of arguments\n");
        return 1;
    }
    return 0;
}

int openFile(char *filename)
{
    int fin=open(filename,O_RDONLY);
    if(fin==-1)
    {
        perror("cannot open input file\n");
    }
    return fin;
}

DIR *openDir(char *dirname)
{
    DIR *din=opendir(dirname);
    if(din==NULL)
    {
        perror("cannot open input file\n");
    }
    return din;
}

int isBMP(char *name)
{
    char *ext = strchr(name,'.');
    if(strcmp(ext,".bmp")!=0)
    {
        perror("file is not bmp");     //merge programu si daca da eroare
        return 0;
    }
    return 1;
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

char* getUserRights(int fileIN, int fileSTATS)
{
    char rights[4];
    if(fileInfo.st_mode & S_IRUSR)
    {
        rights[0]='R';
    }
    else
    {
        rights[0]='-';
    }
    if(fileInfo.st_mode & S_IWUSR)
    {
        rights[1]='W';
    }
    else
    {
        rights[1]='-';
    }
    if(fileInfo.st_mode & S_IXUSR)
    {
        rights[2]='X';
    }
    else
    {
        rights[2]='-';
    }
    return strdup(rights);
}

char* getGroupRights(int fileIN, int fileSTATS)
{
    char rights[4];
    if(fileInfo.st_mode & S_IRGRP)
    {
        rights[0]='R';
    }
    else
    {
        rights[0]='-';
    }
    if(fileInfo.st_mode & S_IWGRP)
    {
        rights[1]='W';
    }
    else
    {
        rights[1]='-';
    }
    if(fileInfo.st_mode & S_IXGRP)
    {
        rights[2]='X';
    }
    else
    {
        rights[2]='-';
    }
    return strdup(rights);
}

char* getOtherRights(int fileIN, int fileSTATS)
{
    char rights[4];
    if(fileInfo.st_mode & S_IROTH)
    {
        rights[0]='R';
    }
    else
    {
        rights[0]='-';
    }
    if(fileInfo.st_mode & S_IWOTH)
    {
        rights[1]='W';
    }
    else
    {
        rights[1]='-';
    }
    if(fileInfo.st_mode & S_IXOTH)
    {
        rights[2]='X';
    }
    else
    {
        rights[2]='-';
    }
    return strdup(rights);
}

void outputBMP(int fileIN,int fileOUT, int fileSTATS, char *name)
{
    char output[OUT_SIZE];
    int height=0,width=0;
    
    lseek(fileIN,18,SEEK_SET);
    read(fileIN, &width, sizeof(int));
    read(fileIN, &height, sizeof(int));

    char *last_modif = ctime(&fileInfo.st_mtime);
    char *rightsUSR = getUserRights(fileIN, fileSTATS);
    char *rightsGRP = getGroupRights(fileIN, fileSTATS);
    char *rightsOTH = getOtherRights(fileIN, fileSTATS);
    
    sprintf(output,"Nume Fisier: %s \nInaltime: %dpx \nLatime: %dpx \nDimensiune: %ldkb \nUser ID: %d \nUltima Modificare: %sContor Legaturi: %ld \nDrepturi User: %s \nDrepturi Gruo: %s \nDrepturi Altii: %s\n\n", name, height, width,fileInfo.st_size,fileInfo.st_uid,last_modif,fileInfo.st_nlink,rightsUSR,rightsGRP,rightsOTH);

    if (write(fileOUT,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    free(rightsUSR);
    free(rightsGRP);
    free(rightsOTH);
}

int main(int argc, char *argv[])
{
    if(argCheck(argc))
    {
        exit(-1);
    }

    DIR *dir=openDir(argv[1]);

    int fout=outputFileCreator();

    while((dirInfo=readdir(dir))!=NULL)
    {
        //printf("%s\n",dirInfo->d_name);
        if(isBMP(dirInfo->d_name))
        {
            char *filePath = (char*)malloc(strlen(argv[1])+2+strlen(dirInfo->d_name));
            strcpy(filePath,argv[1]);
            filePath=strcat(filePath,"/");
            filePath=strcat(filePath,dirInfo->d_name);
            int file=openFile(filePath);
            int fs=getFileStat(file);
            outputBMP(file,fout,fs,dirInfo->d_name);
            free(filePath);
            close(file);
        }
    }

    close(fout);
    closedir(dir);

    return 0;
}