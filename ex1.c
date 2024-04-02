#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
int main()
{
int file=open("vishnu.txt",O_CREAT);
if(file==-1)
{
printf("unable to create a file\n");
exit(0);
}
if(close(file)==-1)
{
printf("unable to close file\n");
exit(0);
}

file=open("vishnu.txt",O_WRONLY|O_APPEND);
if(file==-1)
{
printf("unable to open a file\n");
exit(0);
}
char* data="hello,world!!!";
int a=write(file,data,strlen(data));
if(a==-1)
{
printf("unable to write data:");
exit(0);
}
if(close(file)==-1)
{
printf("unable to close a file\n");
exit(0);
}
printf("data is written successfully...\n");
}
