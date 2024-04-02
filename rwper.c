#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
int main()
{
int file=open("vishnu.txt",O_CREAT,S_IRUSR);
if(file==-1)
{
printf("unable to create a file");
exit(0);
}
if(close(file)==-1)
{
printf("unable to close a file\n");
exit(0);
}

if(chmod("vishnu.txt",S_IRUSR|S_IWUSR)==-1)
{
printf("unable to change permissions\n");
exit(0);
}
file=open("vishnu.txt",O_WRONLY|O_APPEND);
if(file==-1)
{
printf("unable to open in write\n");
exit(0);
}

char* data="hello, world!!";
int a=write(file,data,strlen(data));
if(a==-1)
{
printf("unable to write data:");
exit(0);
}

if(close(file)==-1)
{
printf("unable to close file\n");
exit(0);
}
printf("completed successfully\n");
}
