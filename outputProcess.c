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
typedef struct{
	char device;
	char devicePath[255];
}DEVICE;

typedef struct{
	char fnd[4];	//size = 4
	char led;
	char revBuf[250];
}REV_MSG;

DEVICE connectToFNDDevice(DEVICE devFnd);
DEVICE connectToLEDDevice(DEVICE devLed);


void fndOut(DEVICE devFnd, int fpOut);
void ledOut(DEVICE devLed, int fpOut);

REV_MSG revMsg;
int main(void)
{
	int fpOut;
	int n, fd[2];
//	char buf[255];
	DEVICE fndDev, ledDev;
//	REC_MSG revMsg;
	if((fpOut=open(PIPE_OUTPUT,O_RDONLY))<0)
	{
		perror("open error:");
		exit(EXIT_FAILURE);
	}
//	clockMode(fpOut);
	fndDev = connectToFNDDevice(fndDev);
	ledDev = connectToLEDDevice(ledDev);
	while(1)
	{
		//errer check
		memset(&(revMsg.fnd[0]),0x00,255);
//		printf("empty? %d",revMsg.revBuf[232]);
		n=read(fpOut,&(revMsg.fnd[0]),255);
//		memcpy(&(devFnd.device),&revMsg,255); 
		
		fndOut(fndDev, fpOut);
		ledOut(ledDev, fpOut);		
//		fprintf(stderr, "%d",revMsg.device);
//		fprintf(stderr, "%d\n",revMsg.revBuf[0]);
//		fprintf(stderr, "hello output\n");
	}

	
	
	return 0;
}
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
void fndOut(DEVICE fndDev, int fpOut)
{
	int dev;
	unsigned char data[4];
	unsigned char retval;
	int i,n;
	int str_size;
//	char rec_buf[255];


/*
	dev = open(FND_DEVICE, O_RDWR);
    	if (dev<0) {
        	printf("Device open error : %s\n",FND_DEVICE);
       		exit(1);
    	}
*/


//	while(1)
//	{
//		memset(rec_buf,0x00,255);
//		n=read(fpOut,fndDev.revBuf,255);
		
		memset(data,0,sizeof(data));
		memcpy(data,revMsg.fnd,4);
//		printf("%s\n",revMsg.revBuf);
//		printf("z");
		printf("z%d %d %d %dz\n",data[0],data[1],data[2],data[3]);
  //  		printf("z");
		retval=write(fndDev.device,&data,4);	
    		if(retval<0) {
        		printf("Write Error!\n");
        //		return -1;
    		}

//		memset(data,0,sizeof(data));
//	}

}

void ledOut(DEVICE ledDev, int fpOut)
{
//	int dev;
	unsigned char data;
	unsigned char retval;
/*
	if(argc!=2) {
		printf("please input the parameter! \n");
		printf("ex)./test_led a1\n");
		return -1;
	}
*/
/*	data = atoi(argv[1]);
	if((data<0)||(data>8))
	{
		printf("Invalid range!\n");
        exit(1);
    }
*/
/*
\    ledDev.device = open(LED_DEVICE, O_RDWR);
    if (ledDev.device<0) {
        printf("Device open error : %s\n",LED_DEVICE);
        exit(1);
    }
*/
    retval=write(ledDev.device,&revMsg.led,1);	
    if(retval<0) {
        printf("Write Error!\n");
    //    return -1;
    }

//    sleep(1);

    data=0;
    retval=read(ledDev.device,&data,1);
    if(retval<0) {
        printf("Read Error!\n");
      //  return -1;
    }
    printf("Current LED Value : %d\n",data);

    printf("\n");



}
