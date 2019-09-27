#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
int main()
{	int pid=0;
	char f_path[50];
	scanf(" %d ",&pid);
	scanf("%s",f_path);
	long int mint = syscall(354, pid,f_path);
	printf("System call sys_hello returned %ld \n" , mint);
	return 0;
}
