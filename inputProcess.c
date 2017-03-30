#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
//#include<linux/input.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<termios.h>
#include<unistd.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

#define PIPE_INPUT "input_pipeLine" //input fifo pipe name
#define PIPE_OUTPUT "output_pipeLine" //input fifo pipe name

#define MAX_BUTTON 9

#define RK_BUF 64
#define KEY_RELEASE 0
#define KEY_PRESS 1
#define HOME 102
#define BACK 158
#define PROG 116
#define VOL_UP 115
#define VOL_DOWN 114

typedef struct{
	int device;
	char devicePath[255];


}DEVICE;


void readKey(DEVICE rkDevice, int fpIn);
void clockMode(int fpIn);
void pushSwitch(int fpIn);
void user_signaml1(int sig);

DEVICE connectToRKDevice(DEVICE rkDevice);
DEVICE connectToSWDevice(DEVICE swDevice);

unsigned char quit = 0;
int fpIn;
char send_buf[255];



int main(void)
{
	
	pid_t pid;
	int n, fd[2];
	DEVICE rkDevice, swDevice;	

	//input is from switch 

    if((fpIn=open(PIPE_INPUT,O_WRONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}

	connectToRKDevice(rkDevice);
//	connectToDevice(swDevice);

//	readKey(fpIn);
//	clockMode(fpIn);
		
	//send_buf : first byte-> flag of input kind

	while(1){	
		usleep(400000);
//		memset(buf,0x00,255);
//		sprintf(buf, "Hello Main Process input process is %d\n",getpid());
 	    
	//	clockMode(fpIn);
		readKey(rkDevice,fpIn);

		write(fpIn,send_buf,255);

	}
	
	
	return 0;
}

DEVICE connectToRKDevice(DEVICE rkDevice)
{

	memset(rkDevice.devicePath,0x00,255);
	strcat(rkDevice.devicePath,"/dev/input/event0");

	if((rkDevice.device = open (rkDevice.devicePath, O_RDONLY)) == -1) {
		printf ("%s is not a vaild device.n", device);
	}

}

void readKey(DEVICE rkDevice, int fpIn)
{

	struct input_event ev[RK_BUF];
	int fd, rd, value, size = sizeof (struct input_event);
	char name[256] = "Unknown";
//	char send_buf[255];
/*&
	char* device = "/dev/input/event0";
	if((fd = open (device, O_RDONLY)) == -1) {
		printf ("%s is not a vaild device.n", device);
	}
*/
	// ioctl (fd, EVIOCGNAME (sizeof (name)), name);
	// printf ("Reading From : %s (%s)n", device, name);
/*
	while (1){
		usleep(400000);
*/
		

	if ((rd = read (fd, ev, size * RK_BUF)) < size)
	{
		printf("read()");  
		//	return (0);     
	}

	value = ev[0].value;

	if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){ // Only read the key press event
		printf ("code%d\n", (ev[1].code));
	}
	if( value == KEY_RELEASE ) {
		printf ("key release\n");
	} else 	if( value == KEY_PRESS ) {
		printf ("key press\n");
	

		//ev[0].code size : 2byte
		printf ("Type[%d] Value[%d] Code[%d], size:%d\n", ev[0].type, ev[0].value, (ev[0].code), sizeof(ev[0].code));
		
		memset(send_buf,0x00,255);
		memcpy(send_buf,&ev[0].code,sizeof(ev[0].code));
	//	send to mainprocess	
	//	write(fpIn,send_buf,255);
		
		}

		//ev[0].code size : 2byte
	//	printf ("Type[%d] Value[%d] Code[%d], size:%d\n", ev[0].type, ev[0].value, (ev[0].code), sizeof(ev[0].code));
//		memset(send_buf,0x00,255);			
//		memcpy(send_buf,&ev[0].code,sizeof(ev[0].code));
		//send to mainprocess	
//		write(fpIn,send_buf,255);
	


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


































