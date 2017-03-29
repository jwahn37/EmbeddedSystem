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

#define FND_DEVICE "/dev/fpga_fnd"
void clockMode(int fpOut);

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
	clockMode(fpOut);
		
/*
	while(1)
	{
		memset(buf,0x00,255);
		n=read(fpOut,buf,255);
		fprintf(stderr, "%s",buf);
		fprintf(stderr, "hello output\n");
	}
*/	
	
	return 0;
}

void clockMode(int fpOut)
{
	int dev;
	unsigned char data[4];
	unsigned char retval;
	int i,n;
	int str_size;
	char rec_buf[255];



	dev = open(FND_DEVICE, O_RDWR);
    	if (dev<0) {
        	printf("Device open error : %s\n",FND_DEVICE);
       		exit(1);
    	}



	while(1)
	{
		memset(rec_buf,0x00,255);
		n=read(fpOut,rec_buf,255);
		
		memset(data,0,sizeof(data));
		memcpy(data,rec_buf,4);

		printf("%d %d %d %d\n",data[0],data[1],data[2],data[3]);
    		retval=write(dev,&data,4);	
    		if(retval<0) {
        		printf("Write Error!\n");
        //		return -1;
    		}

		memset(data,0,sizeof(data));
	}
}

