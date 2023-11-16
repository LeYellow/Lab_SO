#include<stdio.h>
#include<stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(strcmp(argv[1],"a")==0)
    printf("Student: Tudor, Grupa 6.1, Anul 3\n");
    
    if(strcmp(argv[1],"b")==0)
    printf("Student: Dani, Grupa 6.1, Anul 3\n");

    if(strcmp(argv[1],"c")==0)
    printf("Student: Cristi, Grupa 6.1, Anul 3\n");
    
    return 0;
}