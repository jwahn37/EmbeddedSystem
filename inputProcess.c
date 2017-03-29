#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<signal.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

#define PIPE_INPUT "input_pipeLine" //input fifo pipe name
#define PIPE_OUTPUT "output_pipeLine" //input fifo pipe name

#define MAX_BUTTON 9

void clockMode(int fpIn);
void pushSwitch(int fpIn);
void user_signaml1(int sig);

unsigned char quit = 0;

int main(void)
{
	int fpIn;
	pid_t pid;
	int n, fd[2];
//	char buf[255];

	//input is from switch 

    if((fpIn=open(PIPE_INPUT,O_WRONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}

	clockMode(fpIn);
		
/*
	while(1){	
		memset(buf,0x00,255);
//		sprintf(buf, "Hello Main Process input process is %d\n",getpid());
		write(fpIn,buf,strlen(buf));
		sleep(1);
	}
*/	
	
	return 0;
}

void user_signal1(int sig)
{
	quit = 1;
}

void clockMode(int fpIn)
{
//	char buf[256];
	
	pushSwitch(fpIn);
/*
	while(1){	
		memset(buf,0x00,255);
		sprintf(buf, "Hello Main Process input process is %d\n",getpid());
		write(fpIn,buf,strlen(buf));
		sleep(1);
	}
*/
}
void pushSwitch(int fpIn)
{
	int dev;
	int i;
	int buff_size;
	
	unsigned char push_sw_buff[MAX_BUTTON];
	char send_buf[255];
	char chFlag=0;

	//open device driver
	dev = open("/dev/fpga_push_switch", O_RDWR);

	//device error
	if(dev<0)		 
	{
		printf("Device Open Error\n");
		close(dev);
//		return -1;
	}

	(void)signal(SIGINT, user_signal1);
	
	buff_size = sizeof(push_sw_buff);
	printf("Press <ctrl+c> to quit. \n");

	while(!quit){
		usleep(400000);
		memset(send_buf,0x00,255);
		read(dev, &push_sw_buff, MAX_BUTTON);
		

		//first character is change flag
		/*
		if(strncmp(send_buf+sizeof(char), push_sw_buff, 9)!=0)//change value
		{
		//	if(send_buf[0]==0)	send_buf[0]=1;
		//	else			send_buf[0]=0;
			send_buf[0]=1;
		}
		*/
		memcpy(send_buf, push_sw_buff, MAX_BUTTON);
		
		printf("input process : ");
		for(i=0;i<=MAX_BUTTON;i++)
			printf("[%d] ",send_buf[i]);
		printf("\n");

		
		
		//first character : variant flag
		write(fpIn,send_buf,255);
	}
	
	close(dev);
	
}


































