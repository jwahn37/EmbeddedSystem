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
	int fpOut;
	int n, fd[2];
	char buf[255];

	if((fpOut=open(PIPE_OUTPUT,O_RDONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}
		
	while(1)
	{
		memset(buf,0x00,255);
		n=read(fpOut,buf,255);
		fprintf(stderr, "%s",buf);
		fprintf(stderr, "hello output\n");
	}
	
	
	return 0;
}



