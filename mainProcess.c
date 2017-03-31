#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
//#include<msg.h>
#include<sys/stat.h>
#include<fcntl.h>

#include "modules.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

#define PIPE_INPUT "input_pipeLine" //input fifo pipe name
#define PIPE_OUTPUT "output_pipeLine" //input fifo pipe name

#define INPUT_PROCESS "inputProcess" //input process
#define OUTPUT_PROCESS "outputProcess" //output process

#define SW_DV 2	//msg was from this device
#define RK_DV 1
#define NO_DV 0 
#define FND_DEV 3

#define HOME 102	//read key
#define BACK 158
#define PROG 116
#define VOL_UP 115
#define VOL_DOWN 114

#define MODE_NUM 1
/*
typedef struct {
	char device;
	unsigned char readKey;
	char switchB[9];
	char buf[244];
//	char msgBuf[254];
}REV_MSG;
*/
/*

typedef struct{
//	char device;
	char fnd[4];
	unsigned char led;
	char buf[250];
}SEND_MSG;
*/
void makeFIFOPipe();
void main_process(int fpIn, int fpOut);
int mainKey(REV_MSG revMsg);
//void input_process(char* buf,int fpIn);
//void output_process(char* buf,int fpOut);

SEND_MSG sendMsg;

int main(void)
{
	int fpIn, fpOut;
	pid_t pid;
	int n, fd[2];
//	char buf[255];
	char path[255];

	makeFIFOPipe();
//printf("%s\n",strcat("./",OUTPUT_PROCESS));	
	//make three process by fork
	if((pid=fork()))
	{
		if(pid==-1){
			perror("fork failed");
			exit(EXIT_FAILURE);
		}

		else if((pid=fork()))
		{
			
			if(pid==-1){
			perror("fork failed");
			exit(EXIT_FAILURE);
			}
			//main process(child2)
		//		printf("%s\n",strcat("./",OUTPUT_PROCESS));	
			main_process(fpIn, fpOut);
		}
		else{
			//output process(parents)
			//output_process(buf,fpOut);
	//	printf("%s\n",strcat("./",OUTPUT_PROCESS));	
		memset(path,0x00, 255);
		strcat(path,"./");
		strcat(path,OUTPUT_PROCESS);
		execl(path,OUTPUT_PROCESS,NULL,(char*) 0);
		
		}

	}
	else{
		//input process(chlid1)
		//input_process(buf,fpIn)	;
	//	printf("%s\n",strcat("./",INPUT_PROCESS));	
		memset(path,0x00, 255);
		strcat(path,"./");
		strcat(path,INPUT_PROCESS);
		execl(path,INPUT_PROCESS,NULL,(char*) 0);
		//execl(strcat("./",INPUT_PROCESS),INPUT_PROCESS,NULL,(char*) 0);
	}
	return 0;
}

void makeFIFOPipe()
{
//make pipe_input
	if( access(PIPE_INPUT, F_OK)==-1)	//file exist check
	{
		if(mkfifo(PIPE_INPUT, S_IRUSR|S_IWUSR)!=0)
//	if(mknod(PIPE_INPUT,S_IFIFO,0))
		{
			perror("mkfifo fauilure : ");
		}
	}
	
	//make pipe2
	if( access(PIPE_OUTPUT, F_OK)==-1) //file exist check
	{
		if(mkfifo(PIPE_OUTPUT, S_IRUSR|S_IWUSR)!=0)
//	if(mknod(PIPE_OUTPUT, S_IFIFO,0))	
		{
			perror("mkfifo fauilure : ");
		}
	}

}

void main_process(int fpIn, int fpOut)
{
	int n;
	int i;
 	char buf[255];
	char output[5]={1,2,3,4,0};
	REVMSG revMsg;
	int mode;

	if((fpIn=open(PIPE_INPUT,O_RDONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}
		
	if((fpOut=open(PIPE_OUTPUT,O_WRONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}
	printf("main\n");
	while(1)
	{
//		memset(buf,0x00, 255);
//		n=read(fpIn,buf,255);
		
		//read from input process
		memset(&(revMsg.device),0x00,255);
		n=read(fpIn,&(revMsg.device),255);	

	/*	
		printf("main : ");
		printf("from %d , ",revMsg.device);
		for(i=0;i<9;i++)
			printf("[%d] ",revMsg.switchB[i]);
		printf("\n");
*/
		//
		mode=mainKey(revMsg);
		if(mode==1){sendMsg=clockMode(sendMsg,revMsg);}
		if(mode==2){}
		if(mode==3){}
		if(mode==4){}
		//...
			
		memset(buf,0x00,255);
		output[3]=(output[3]+1)%10;
		output[4]=(output[4]+1)%8;
//		buf[0]=FND_DEV;

		//output to process
		memcpy(&(sendMsg.fnd[0]),output,5);
		write(fpOut,&(sendMsg.fnd[0]),255);
		usleep(5000);
	}


}

int mainKey(REV_MSG revMsg)
{
	int mode;
	//if(revMsg.readKey == HOME)	//
	if(revMsg.readKey == BACK)	//program exit
	{
		exit(0); //child process ㅈ종료방법 찾기
	}
	if(revMsg.readKey == PROG)
	{}
	if(revMsg.readKey == VOL_UP) //change mode +
	{
		mode = (mode+1) % MODE_NUM;
	}
	if(revMsg.readKey == VOL_DOWN) //change mode -
	{
		mode = (mode+(MODE_NUM-1)) % MODE_NUM;
	}
	return mode;

}