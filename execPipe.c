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

#define INPUT_PROCESS "inputProcess" //input process
#define OUTPUT_PROCESS "outputProcess" //output process
void makeFIFOPipe();
void main_process(char* buf,int fpIn, int fpOut);
void input_process(char* buf,int fpIn);
void output_process(char* buf,int fpOut);

int main(void)
{
	int fpIn, fpOut;
	pid_t pid;
	int n, fd[2];
	char buf[255];
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
			main_process(buf,fpIn, fpOut);
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

void main_process(char* buf, int fpIn, int fpOut)
{
	int n;
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
	while(1)
	{
		memset(buf,0x00, 255);
		n=read(fpIn,buf,255);
		//		fprintf(stderr, "%s",buf);
				
		write(fpOut,buf,strlen(buf));
	}


}
