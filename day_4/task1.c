#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#define filename "memory.txt"
#define filesize 4096

int main()
{
	struct stat file_stat;
int file=open(filename,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
if(file==-1)
{
printf("unable to create a file");
exit(0);
}

if(ftruncate(file,filesize)==-1)
{
printf("truncate error");
close(file);
exit(0);
}

if(fstat(file,&file_stat)==-1)
{
printf("stat error");
close(file);
}

char *mem=mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,file,0);
if(mem==MAP_FAILED)
{
printf("map error");
close(file);
exit(0);
}

strcpy(mem,"hello,world!!!");

if(msync(mem,filesize,MS_SYNC)==-1)
{
printf("sync error");
close(file);
exit(0);
}


if(munmap(mem,filesize)==-1)
{
printf("unmap error");
close(file);
exit(0);
}
close(file);
printf("completed successfully\n");
}
