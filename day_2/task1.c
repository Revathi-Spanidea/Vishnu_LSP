#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	int i,n;
	pid_t pid;
	char *a[]={"ls","pwd","date"};
	n=sizeof(a)/sizeof(a[0]);
	for(i=0;i<n;i++)
	{
		pid=fork();
		if(pid==-1)
		{
			printf("unable to create child process\n");
			exit(0);
		}
		if(pid==0)
		{
			printf("child process %d is %s\n",getpid(),a[i]);
			execlp(a[i],a[i],NULL);
			printf("execlp error\n");
			exit(0);
		}
	}

	for(i=0;i<n;i++)
	{
		wait(NULL);
	}

	printf("all commands are executed successfully...\n");
}
