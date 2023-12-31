#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#define OUT_SIZE 512
#define SIZE 50

struct stat fileInfo;
struct dirent *dirInfo;

int argCheck(int x)
{
    if(x!=3)
    {
        perror("Forma corecta: ./nume <dir intrare> <dir iesire>\n");
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

int openFileForWrite(char *filename)
{
    int fin=open(filename,O_RDWR);
    if(fin==-1)
    {
        perror("cannot open input file\n");
    }
    return fin;
}

int outputFileCreator(char *dir, char *file)
{
    char pathName[SIZE];
    strcpy(pathName,dir);
    strcat(pathName,"/");
    strcat(pathName,file);
    int fout=open(pathName,O_WRONLY | O_CREAT | O_TRUNC ,S_IRUSR | S_IWUSR);
    if(fout==-1)
    {
        perror("invalid output file\n");
    }
    return fout;
}

DIR *openDir(char *dirname)
{
    DIR *din=opendir(dirname);
    if(din==NULL)
    {
        perror("cannot open directory\n");
    }
    return din;
}

int isBMP(char *name)
{
    char *ext = strchr(name,'.');
    if(ext==NULL)
    {
        return 0;
    }
    if((strcmp(ext,".bmp")!=0) || ext==NULL)
    {
        //perror("file is not bmp");     //merge programu si daca da eroare
        return 0;
    }
    return 1;
}

int isREG(int type)
{
    //DT_REG = 8
    if(type == 8)
    {
        return 1;
    }
    return 0;
}

int isDIR(int type)
{
    //DT_DIR = 4
    if(type == 4)
    {
        return 1;
    }
    return 0;
}

int isLINK(int type)
{
    //DT_LNK =10
    if(type == 10)
    {
        return 1;
    }
    return 0;
}

int isNullFolder(char *aux)
{
    if(!strcmp(aux,".") || !strcmp(aux,".."))
    {
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

char* getOutputFileName()
{
    char aux[SIZE];
    strcpy(aux,dirInfo->d_name);
    char *token=strtok(aux,".");
    strcat(token,"_statistica.txt");
    return strdup(aux);
}

char* getOutputFileName2()
{
    char aux[SIZE];
    strcpy(aux,dirInfo->d_name);
    strcat(aux,"_statistica.txt");
    return strdup(aux);
}

char* getFilePath(char *name)
{
    int alloc=strlen(name)+2+strlen(dirInfo->d_name);
    char *filePath = (char*)malloc(alloc);
    strcpy(filePath,name);
    filePath=strcat(filePath,"/");
    filePath=strcat(filePath,dirInfo->d_name);
    return strdup(filePath);
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
    
    sprintf(output,"Nume Fisier: %s \nInaltime: %dpx \nLatime: %dpx \nDimensiune: %ldb \nUser ID: %d \nUltima Modificare: %sContor Legaturi: %ld \nDrepturi User: %s \nDrepturi Grup: %s \nDrepturi Altii: %s\n\n", name, height, width,fileInfo.st_size,fileInfo.st_uid,last_modif,fileInfo.st_nlink,rightsUSR,rightsGRP,rightsOTH);

    if (write(fileOUT,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    free(rightsUSR);
    free(rightsGRP);
    free(rightsOTH);
}

void outputDIR(int fileIN, int fileOUT, int fileSTATS, char *name)
{
    char output[OUT_SIZE];
    char *rightsUSR = getUserRights(fileIN, fileSTATS);
    char *rightsGRP = getGroupRights(fileIN, fileSTATS);
    char *rightsOTH = getOtherRights(fileIN, fileSTATS);

    sprintf(output,"Nume Director: %s \nUser ID: %d \nDrepturi User: %s \nDrepturi Grup: %s \nDrepturi Altii: %s\n\n", name,fileInfo.st_uid,rightsUSR,rightsGRP,rightsOTH);

    if (write(fileOUT,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    free(rightsUSR);
    free(rightsGRP);
    free(rightsOTH);
}

void outputLINK(int fileIN, int fileOUT, int fileSTATS, char *name)
{
    char output[OUT_SIZE];
    char *rightsUSR = getUserRights(fileIN, fileSTATS);
    char *rightsGRP = getGroupRights(fileIN, fileSTATS);
    char *rightsOTH = getOtherRights(fileIN, fileSTATS);
    sprintf(output,"Nume Legatura: %s \nDimensiune Legatura: %ldb \nDimensiune Target: %db \nDrepturi User: %s \nDrepturi Grup: %s \nDrepturi Altii: %s\n\n", name,fileInfo.st_size,0,rightsUSR,rightsGRP,rightsOTH);

    if (write(fileOUT,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    free(rightsUSR);
    free(rightsGRP);
    free(rightsOTH);
}

void outputREG(int fileIN,int fileOUT, int fileSTATS, char *name)
{
    char output[OUT_SIZE];
    
    char *last_modif = ctime(&fileInfo.st_mtime);
    char *rightsUSR = getUserRights(fileIN, fileSTATS);
    char *rightsGRP = getGroupRights(fileIN, fileSTATS);
    char *rightsOTH = getOtherRights(fileIN, fileSTATS);
    
    sprintf(output,"Nume Fisier: %s \nDimensiune: %ldb \nUser ID: %d \nUltima Modificare: %sContor Legaturi: %ld \nDrepturi User: %s \nDrepturi Grup: %s \nDrepturi Altii: %s\n\n", name,fileInfo.st_size,fileInfo.st_uid,last_modif,fileInfo.st_nlink,rightsUSR,rightsGRP,rightsOTH);

    if (write(fileOUT,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }

    free(rightsUSR);
    free(rightsGRP);
    free(rightsOTH);
}

void outputLinesNumber(int fileOUT, int nrlinii)
{
    char output[OUT_SIZE];
    sprintf(output, "`%s` a returnat %d linii\n", dirInfo->d_name,nrlinii);
    if (write(fileOUT,output,strlen(output))==-1)
    {
        perror("Error writing to output file");
    }
}

void convertGreyscale(char *aux)    //24 biti only
{
    int fin=openFileForWrite(aux);
    int w, h, offset,bitCount;
    uint8_t pixel[3],greyP;

    lseek(fin,10,SEEK_SET);
    read(fin,&offset,sizeof(offset));
    lseek(fin,18,SEEK_SET);
    read(fin, &w, sizeof(int));
    read(fin, &h, sizeof(int));
    lseek(fin,2,SEEK_CUR);
    read(fin,&bitCount,sizeof(bitCount));

    if(bitCount==24)
    {
        for (int j = 0; j < h*w; j++) {   
            read(fin, &pixel, sizeof(pixel));       //blue = pixel[0];green = pixel[1];red = pixel[2];
            
            greyP=0.299*pixel[2] + 0.587*pixel[1] + 0.144*pixel[0];
            pixel[0]=pixel[1]=pixel[2]=greyP;
            
            lseek(fin,-3,SEEK_CUR);
            write(fin,pixel,sizeof(pixel));
        }
        close(fin);
    }
    else
    {
        printf("Bit Count diferit fata de cel programat pentru %s",aux);
        close(fin);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    char *fp, *outFileName;
    int sta;
    if(argCheck(argc))
    {
        exit(-1);
    }

    DIR *din=openDir(argv[1]);
    DIR *dout=openDir(argv[2]);

    int fout=outputFileCreator(argv[2],"statistica.txt");
    
    while((dirInfo=readdir(din))!=NULL)
    {
        if(isNullFolder(dirInfo->d_name))
        {
            continue;
        }
        else if(isBMP(dirInfo->d_name))
        {
            int fork_bmp = fork();
            if(fork_bmp == 0)
            {
                outFileName=getOutputFileName();
                int bmpfile=outputFileCreator(argv[2],outFileName);
                fp = getFilePath(argv[1]);
                int file=openFile(fp);
                int fs=getFileStat(file);
                outputBMP(file,bmpfile,fs,dirInfo->d_name);
                free(fp);
                close(file);
                close(bmpfile);
                outputLinesNumber(fout,10);
                exit(EXIT_SUCCESS);
            }
            printf("S-a incheiat procesul cu pid: %d si codul: %d pentru fisierul: %s\n", wait(&sta), sta,dirInfo->d_name);
            
            int fork_greyscale=0;
            if(fork_greyscale == 0)
            {
                fp = getFilePath(argv[1]);
                convertGreyscale(fp);
            }
            printf("S-a incheiat procesul de grayscale cu pid: %d si codul: %d pentru fisierul: %s\n", wait(&sta), sta,dirInfo->d_name);
        }
        else if(isDIR(dirInfo->d_type))
        {
            int fork_dir = fork();
            if(fork_dir==0)
            {
                outFileName=getOutputFileName2();
                int dirfile=outputFileCreator(argv[2],outFileName);
                fp = getFilePath(argv[1]);
                int file=openFile(fp);
                int fs=getFileStat(file);
                outputDIR(file,dirfile,fs,dirInfo->d_name);
                free(fp);
                close(file);
                close(dirfile);
                outputLinesNumber(fout,5);
                exit(EXIT_SUCCESS);
            }
            printf("S-a incheiat procesul cu pid: %d si codul: %d pentru fisierul: %s\n", wait(&sta), sta,dirInfo->d_name);
        }
        else if(isLINK(dirInfo->d_type))
        {   
            int fork_link = fork();
            if(fork_link==0)
            {
                outFileName=getOutputFileName2();
                int linkfile=outputFileCreator(argv[2],outFileName);
                fp = getFilePath(argv[1]);
                int file=openFile(fp);
                int fs=getFileStat(file);
                outputLINK(file,linkfile,fs,fp);
                free(fp);
                close(file);
                close(linkfile);
                outputLinesNumber(fout,6);
                exit(EXIT_SUCCESS);
            }
            printf("S-a incheiat procesul cu pid: %d si codul: %d pentru fisierul: %s\n", wait(&sta), sta,dirInfo->d_name);
        }
        else if(isREG(dirInfo->d_type))
        {
            int fork_reg = fork();
            if(fork_reg==0)
            {
                outFileName=getOutputFileName();
                int regfile=outputFileCreator(argv[2],outFileName);
                fp = getFilePath(argv[1]);
                int file=openFile(fp);
                int fs=getFileStat(file);
                outputREG(file,regfile,fs,dirInfo->d_name);
                free(fp);
                close(file);
                close(regfile);
                outputLinesNumber(fout,8);
                exit(EXIT_SUCCESS);
            }
            printf("S-a incheiat procesul cu pid: %d si codul: %d pentru fisierul: %s\n", wait(&sta), sta,dirInfo->d_name);
        
            
        }    
    }

    close(fout);
    closedir(din);
    closedir(dout);

    return 0;
}