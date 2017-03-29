#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

#define PIPE_INPUT "input_pipeLine" //input fifo pipe name
#define PIPE_OUTPUT "output_pipeLine" //input fifo pipe name


int main(void)
{
	int fpIn;
	pid_t pid;
	int n, fd[2];
	char buf[255];

    if((fpIn=open(PIPE_INPUT,O_WRONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}
		
	while(1){

		memset(buf,0x00,255);
		sprintf(buf, "Hello Main Process input process is %d\n",getpid());
		write(fpIn,buf,strlen(buf));
		sleep(1);
	}
	
	
	return 0;
}



