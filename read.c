#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>

int main()
{
int file=open("vishnu.txt",O_CREAT,S_IRUSR);
if(file==-1)
{
printf("unable to create a file");
}

if(close(file)==-1)
{
printf("unable to close the file\n");
exit(0);
}

if(chmod("vishnu.txt",S_IRUSR|S_IWUSR)==-1)
{
printf("unable to change to permissions\n");
exit(0);
}
printf("read only permission is given only to use\n");
printf("read and write permissions are given to owner\n");
} 
