#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

pid_t child1, child2, parent;
int failCount = 0;

int search(int *arr, int start, int end, int target, pid_t parent) 
{
	while(start < end) 
	{
	  if( arr[start] == target ) 
	  {
	    kill(parent, SIGUSR1);
	    exit(0);
	  } else 
	    {
	    start++;
	    }
	}
	kill(parent, SIGUSR2);
	exit(1);
}

void catcher(int signo) 
{
	if(signo == SIGUSR1) 
	{
	  printf("target found\n");
	  kill(child1, SIGTERM);
	  kill(child2, SIGTERM);
	  exit(0);
	}
	else 
	{
	failCount++;
	if(failCount == 2) 
	{
	printf("not found\n");
	kill(child1, SIGTERM);
	kill(child2, SIGTERM);
	exit(1);
	  }
	}
}
	

int main() 
{
	int arr[] = {0, 1, 2, 3, 4, 5};
	signal(SIGUSR1, catcher);
	signal(SIGUSR2, catcher);
	
	pid_t child1 = fork();
	if(child1 == 0) 
	{
	search(arr, 0, 3, 5, parent);
	}
	else 
	{
	  pid_t child2 = fork();
	  if(child2 == 0) {
	   search(arr, 3, 6, 5, parent);
	  }
	}
	for(;;) 
	{
	wait();
	}
}
	  
