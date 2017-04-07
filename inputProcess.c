#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<linux/input.h>
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

#define SW_DV 2
#define RK_DV 1
#define NO_DV 0

//each device driver 
typedef struct{
	int device;
	char devicePath[255];


}DEVICE;

//the message to send to main process 
typedef struct{
	char device;
	unsigned char readKey; //size is 2byte
	char switchB[9]; //size is 9byte
	char sendBuf[244];

}SEND_MSG;


void readKey(DEVICE rkDevice, int fpIn);
void clockMode(int fpIn);
void pushSwitch(DEVICE swDevice, int fpIn);
void user_signaml1(int sig);

DEVICE connectToRKDevice(DEVICE rkDevice);
DEVICE connectToSWDevice(DEVICE swDevice);

SEND_MSG sendMsg;
unsigned char quit = 0;
int fpIn;

// input process conncect the device driver of board 
// and transmit user's input to main process by named pipe

int main(void)
{
	
	pid_t pid;
	int n, fd[2];
	DEVICE rkDevice, swDevice;	
	char comp[9];
	char past[9];
	unsigned char pastR;
	pastR=0;
	memset(comp,0x00,9);
	memset(past,0x00,9);
	//open pipe with write mode
    	if((fpIn=open(PIPE_INPUT,O_WRONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}
	
	//connect to device driver
	rkDevice = connectToRKDevice(rkDevice);
	swDevice = connectToSWDevice(swDevice);
	

	//send user's input to the main process continuously
	while(1){	
		usleep(100000);
 	    	memset(&sendMsg,0x00,255);
		pushSwitch(swDevice,fpIn);
		readKey(rkDevice,fpIn);
	
		//don't permit long clicked button
		if((!memcmp(past,comp,9)) && pastR==0)
		{
			write(fpIn,&sendMsg,255);
		}	
		
		memcpy(past,sendMsg.switchB,9);
		pastR=sendMsg.readKey;
		
	}
	//close device driver
	close(swDevice.device);
	close(rkDevice.device);

	return 0;
}

//connect to readkey device driver
DEVICE connectToRKDevice(DEVICE rkDevice)
{
	int i;
	memset(rkDevice.devicePath,0x00,255);
	memcpy(rkDevice.devicePath,"/dev/input/event0",18);
	
	if((rkDevice.device = open (rkDevice.devicePath, O_RDONLY|O_NONBLOCK)) == -1) {
		printf ("%s is not a vaild device.n", rkDevice.devicePath);
	}
	return rkDevice;

}

//Read user's input to read key device driver and store in variable sendMsg
//by sendMsg, the input go to the main process
void readKey(DEVICE rkDevice, int fpIn)
{

	struct input_event ev[RK_BUF];
	int fd, rd, value, size = sizeof (struct input_event);
	char name[256] = "Unknown";
	
	//read readkey device
	if ((rd = read (rkDevice.device, ev, size * RK_BUF)) < size)
	{
		ev[0].value = KEY_RELEASE; 
	}

	value = ev[0].value;

	if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){ // Only read the key press event
		printf ("code%d\n", (ev[1].code));
	}
	if( value == KEY_PRESS ) {
		//copy the user's input to sendMsg	
		sendMsg.device=RK_DV;
		memset(&(sendMsg.device),0x00,255);
		memcpy(&(sendMsg.readKey),&ev[0].code,sizeof(ev[0].code));
		}
}


void user_signal1(int sig)
{
	quit = 1;
}

//connect to switch device driver
DEVICE connectToSWDevice(DEVICE swDevice)
{
	int i;
	//init swDevice
	memset(swDevice.devicePath,0x00,255);
	memcpy(swDevice.devicePath,"/dev/fpga_push_switch",22);
	printf("%s path \n ",swDevice.devicePath);

	//device driver open
	if((swDevice.device = open (swDevice.devicePath, O_RDWR)) == -1) {
		printf("Device Open Error\n");
		close(swDevice.device);
	}
	(void)signal(SIGINT, user_signal1);

	return swDevice;

}
//read user's input on switch and store in sendMsg. 
//by sendMsg, the input goes to the main process
void pushSwitch(DEVICE swDevice, int fpIn)
{
	int i;
	int buff_size;
	char switchB[9];
	static char pastB[9]={0,0,0,0,0,0,0,0,0};
	unsigned char push_sw_buff[MAX_BUTTON];
	char chFlag=0;
	struct input_event ev[64];
	
	buff_size = sizeof(push_sw_buff);
	printf("Press <ctrl+c> to quit. \n");

	//read user's input to switch
	read(swDevice.device, &push_sw_buff, MAX_BUTTON);
	sendMsg.device=SW_DV;
	
	


	//check if the switch clicked continuously
	//copy the user's input to sendMsg	
	memcpy(sendMsg.switchB, push_sw_buff, MAX_BUTTON);
	memcpy(pastB,sendMsg.switchB,9);		

}


































