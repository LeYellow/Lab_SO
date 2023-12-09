#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>

int main()
{
    int fin=open("dir/pic1.bmp",O_RDWR);
    if(fin==-1)
    {
        perror("cannot open input file\n");
    }

    int w, h, offset,bitCount;
    
    lseek(fin,10,SEEK_SET);
    read(fin,&offset,sizeof(offset));

    lseek(fin,18,SEEK_SET);
    read(fin, &w, sizeof(int));
    read(fin, &h, sizeof(int));
    
    lseek(fin,2,SEEK_CUR);
    read(fin,&bitCount,sizeof(bitCount));

    

    //uint8_t redP, greenP, blueP, greyP;
    //char pixel[3],greyP;
    char pixel[3], greyP;

if(bitCount==24)
{
    lseek(fin,offset,SEEK_SET);
    for (int j = 0; j < h*w; j++) {
            
            read(fin, &pixel, sizeof(pixel));
            //blueP = pixel[0];
            //greenP = pixel[1];
            //redP = pixel[2];
            
            greyP=0.299*pixel[2] + 0.587*pixel[1] + 0.144*pixel[0];
            pixel[0]=pixel[1]=pixel[2]=greyP;

            lseek(fin,-3,SEEK_CUR);
            write(fin,pixel,sizeof(pixel));
            //for(int i=0;i<3;i++)
            //{
                //write(fin, &greyP, sizeof(uint8_t));
            //}
            
    }

    close(fin);
} else {
    printf("Bit Count diferit fata de cel programat");
    close(fin);
    exit(EXIT_FAILURE);
}
    return 0;
}

/*pt pipes
pid1=fork()
if(pid==0)
{
    if bmp->statis
    if fob -> stat + cont fis
    if dir -> stat
    if sim -> stat  
}
else
{
    if(bmp)
    {
        pid2=fork()
        if(pid2==0)
        {
            greyscale
        }
    }
    if(reg)
    {
        pid3=fork()
        if(pid3==0)
        {
            script
        }
    }
}

//parent code - preiau stari 
//5 procese
*/