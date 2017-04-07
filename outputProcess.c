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
#define LED_DEVICE "/dev/fpga_led"
#define LCD_DEVICE "/dev/fpga_text_lcd"
#define DOT_DEVICE "/dev/fpga_dot"

#define LCD_MAXBUF 32
#define LCD_LINEBUF 16

//device variable
typedef struct{
	char device;
	char devicePath[255];
}DEVICE;

//recieved message variable from main process
typedef struct{
	char fnd[4];	//size = 4
	unsigned char led;
	char lcd[33];
	unsigned char dot[10];
	char revBuf[207];
}REV_MSG;

DEVICE connectToFNDDevice(DEVICE devFnd);
DEVICE connectToLEDDevice(DEVICE devLed);
DEVICE connectToLCDDevice(DEVICE devLcd);
DEVICE connectToDOTDevice(DEVICE devDot);

void fndOut(DEVICE devFnd, int fpOut);
void ledOut(DEVICE devLed, int fpOut);
void lcdOut(DEVICE devLcd, int fpOut);
void dotOut(DEVICE devDot, int fpOut);

REV_MSG revMsg;

//outProcess connect with device drive of board and control device with mainprocess command by named pipe
int main(void)
{
	int fpOut;
	int n, fd[2];
	//fpga_number[0] means alphabet 'A', fpga_number[1] means number '1'
	unsigned char fpga_number[2][10] = {
	{0x1c,0x36,0x63,0x63,0x63,0xff,0xff,0x63,0x63,0x63},
	{0x0c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3f,0x3f}
	};
	
	DEVICE fndDev, ledDev, lcdDev, dotDev;
	if((fpOut=open(PIPE_OUTPUT,O_RDONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}
	fndDev = connectToFNDDevice(fndDev);
	ledDev = connectToLEDDevice(ledDev);
	lcdDev = connectToLCDDevice(lcdDev);
	dotDev = connectToDOTDevice(dotDev);
	
	//read from main process and output to device driver
	while(1)
	{
		memset(&(revMsg.fnd[0]),0x00,255);
		n=read(fpOut,&(revMsg.fnd[0]),255);
		
		fndOut(fndDev, fpOut);
		ledOut(ledDev, fpOut);		
		lcdOut(lcdDev, fpOut);
		dotOut(dotDev, fpOut);
	}
	
	return 0;
}

//connect to fnd device driver
DEVICE connectToFNDDevice(DEVICE fndDev)
{
	
	fndDev.device = open(FND_DEVICE, O_RDWR);
    	memset(fndDev.devicePath,0x00,255);
	memcpy(fndDev.devicePath,FND_DEVICE,14);

	if (fndDev.device<0) {
        	printf("Device open error : %s\n",FND_DEVICE);
       		exit(1);
    	}
	return fndDev;
}

//connect to led device driver
DEVICE connectToLEDDevice(DEVICE ledDev)
{
	
	ledDev.device = open(LED_DEVICE, O_RDWR);
    	memset(ledDev.devicePath,0x00,255);
	memcpy(ledDev.devicePath,LED_DEVICE,14);

	if (ledDev.device<0) {
        	printf("Device open error : %s\n",LED_DEVICE);
       		exit(1);
    	}

	return ledDev;
}

//connect to lcd device driver
DEVICE connectToLCDDevice(DEVICE lcdDev)
{
	lcdDev.device = open(LCD_DEVICE, O_WRONLY);
	memset(lcdDev.devicePath,0x00,255);
	memcpy(lcdDev.devicePath,LCD_DEVICE,18);
	
	if(lcdDev.device<0)
	{
		printf("Device open error :  %s\n",LCD_DEVICE);
		exit(1);
	}

	return lcdDev;
}

//connect to dot device driver
DEVICE connectToDOTDevice(DEVICE dotDev)
{
	dotDev.device = open(DOT_DEVICE, O_WRONLY);
	memset(dotDev.devicePath,0x00,255);
	memcpy(dotDev.devicePath,DOT_DEVICE,13);

	if(dotDev.device<0)
	{	
		pritnf("Device open error : %s\n",DOT_DEVICE);
		exit(1);
	}
	return dotDev;
}

//print the revMsg from main process to the fnd device driver
void fndOut(DEVICE fndDev, int fpOut)
{
	int dev;
	unsigned char data[4];
	unsigned char retval;
	int i,n;
	int str_size;

		
	memset(data,0,sizeof(data));
	memcpy(data,revMsg.fnd,4);
	printf("z%d %d %d %dz\n",data[0],data[1],data[2],data[3]);
	retval=write(fndDev.device,&data,4);	
    	if(retval<0) {
        	printf("Write Error!\n");
    	}

}

//print the revMsg from main process to the led device driver
void ledOut(DEVICE ledDev, int fpOut)
{
	unsigned char data;
	unsigned char retval;
    	
	retval=write(ledDev.device,&revMsg.led,1);	
    	if(retval<0) {
        	printf("Write Error!\n");
    }

    	data=0;
    	retval=read(ledDev.device,&data,1);
    	if(retval<0) {
        	printf("Read Error!\n");
    	}
}

//print the revMsg from main process to the lcd device driver
void lcdOut(DEVICE lcdDev, int fpOut)
{
	char string[LCD_MAXBUF];
	memset(string,0x00,LCD_MAXBUF);
	memcpy(string,revMsg.lcd,LCD_MAXBUF);
	write(lcdDev.device,string,LCD_MAXBUF);	
}

//print the revMsg from main process to the dot device driver
void dotOut(DEVICE dotDev, int fpOut)
{
	unsigned char retval=write(dotDev.device, revMsg.dot, sizeof(revMsg.dot));

	if(retval<0){
		printf("Write Error!\n");
	}

}



