#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
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

#define MODE_NUM 5

void makeFIFOPipe();
void main_process(int fpIn, int fpOut,pid_t pid1, pid_t pid2);
int mainKey(REV_MSG revMsg,int mode, pid_t pid1, pid_t pid2);

SEND_MSG sendMsg;

//main process is the major part of this system. 
//with fork, excl, it makes inputProcess and outputProcess
//with pipe IPC, it read data from inputProcess and execute algorithm following the modes
//and write to outputProcess
int main(void)
{
	int fpIn, fpOut;
	pid_t pid,pid2;
	int n, fd[2];
	char path[255];

	//make pipeline
	makeFIFOPipe();

	//make input process and output process
	if((pid=fork()))
	{
		if(pid==-1){
			perror("fork failed");
			exit(EXIT_FAILURE);
		}

		else if((pid2=fork()))
		{
			
			if(pid==-1){
			perror("fork failed");
			exit(EXIT_FAILURE);
			}
			//main process(child2)
			main_process(fpIn, fpOut,pid,pid2);
		}
		else{
			//output process(parents)
		memset(path,0x00, 255);
		strcat(path,"./");
		strcat(path,OUTPUT_PROCESS);
		execl(path,OUTPUT_PROCESS,NULL,(char*) 0);
		
		}

	}
	else{
		//input process(chlid1)
		memset(path,0x00, 255);
		strcat(path,"./");
		strcat(path,INPUT_PROCESS);
		execl(path,INPUT_PROCESS,NULL,(char*) 0);
	}
	return 0;
}

//make ICP named pipe
//here, make input_pipeline which is between input process and main process
//and make output_pipeline which is between output process and main process
void makeFIFOPipe()
{
//make pipe_input
	if( access(PIPE_INPUT, F_OK)==-1)	//file exist check
	{
		if(mkfifo(PIPE_INPUT, S_IRUSR|S_IWUSR)!=0)
		{
			perror("mkfifo fauilure : ");
		}
	}
	
	//make pipe2
	if( access(PIPE_OUTPUT, F_OK)==-1) //file exist check
	{
		if(mkfifo(PIPE_OUTPUT, S_IRUSR|S_IWUSR)!=0)
		{
			perror("mkfifo fauilure : ");
		}
	}

}

//main process 
//from the readkey, it receive user's request and execute modules
//executing modules, it send data to output process
void main_process(int fpIn, int fpOut, pid_t pid1, pid_t pid2)
{
	int n;
	int i;
 	char buf[255];
	char output[5]={1,2,3,4,0};
	REV_MSG revMsg;
	int pastMode=100, mode=100;
	char clockChFlag=0;
	time_t timeS,timeS2;	
	char numbState = DECIMAL_NUMB;

	//open the pipe
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
	mode=0;
	while(1)
	{
		//read pipe from inputProcess	
		memset(&(revMsg.device),0x00,255);
		n=read(fpIn,&(revMsg.device),255);	

		//call mainkey
		mode=mainKey(revMsg,mode,pid1, pid2);
		printf("mode : %d read key : %d\n",mode,revMsg.readKey);
	
		//init mode
		if(pastMode!=mode) 
		{	printf("button is 9999\n");
			revMsg.switchB[0]=9;	
			revMsg.switchB[1]=9;	
			revMsg.switchB[2]=9;	
			revMsg.switchB[3]=9;
			memset(sendMsg.fnd,0x00,4);
			sendMsg.led=0;
			memset(sendMsg.lcd,0x00,32);
			memset(sendMsg.dot,0x00,10);	
		}

		//clock mode
		if(mode==0){
			if(clockChFlag==0)
				timeS=time(NULL);
			sendMsg=clockMode(sendMsg,revMsg,&clockChFlag,&timeS);
			pastMode=mode;
		}

		//counter mode
		if(mode==1){
			sendMsg=counterMode(sendMsg,revMsg,&numbState);
			pastMode=mode;
		}

		//text editor
		if(mode==2){
			sendMsg=textEditor(sendMsg, revMsg);
			pastMode=mode;
		}

		//draw board
		if(mode==3){
			timeS2=time(NULL);
			sendMsg=drawBoard(sendMsg, revMsg,&timeS);
			pastMode=mode;
		}
		//calculator
		if(mode==4){
			sendMsg=calculator(sendMsg, revMsg);
			pastMode=mode;
		}
			
		write(fpOut,&(sendMsg.fnd[0]),255);
		usleep(5000);
	}


}

//mainKey execute the command from user's mainkey device input
//BACK : exit program
//VOL + , VOL - : change mode
int mainKey(REV_MSG revMsg,int mode, pid_t pid1, pid_t pid2)
{
	if(revMsg.readKey == BACK)	//program exit
	{
		
		kill(pid1, SIGKILL);	
		kill(pid2, SIGKILL);
		exit(0); 
	}
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
