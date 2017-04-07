#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include "modules.h"

#define LED_1 128
#define LED_3_4 48
#define LED_3 32
#define LED_4 16

SEND_MSG incHour(SEND_MSG sendMsg);
SEND_MSG incMin(SEND_MSG sendMsg);
SEND_MSG ledClock(SEND_MSG sendMsg);
SEND_MSG boardTime(SEND_MSG sendMsg,int incH,int incM);


//clockMode print time and state by LED and FND
//with user's input, the time can be changed
//if the user change the time, time should continuosly flow 
SEND_MSG clockMode(SEND_MSG sendMsg, REV_MSG revMsg,char* changeFlag,time_t* timerS){


	time_t timerF=0;
	float gap;
	char input;
	char switchB[4];
	int i;  
    	static int incH=0,incM=0;
  
	//check init mode 
	memset(switchB,0x00,4);
	memcpy(switchB,revMsg.switchB,4);
	for( i=0;i<4;i++)	switchB[i]=(char)(switchB[i]+'0');	//change to string from integer

	printf("clock button : %c %c %c %c// %d\n",switchB[0],switchB[1],switchB[2],switchB[3],revMsg.switchB[4]);	
	printf("strncmp : %d\n",strncmp(switchB,"9999",4));
	//if init mode
	if(!strncmp(switchB,"9999",4)) //init
	{
		printf("LED init\n");
    		sendMsg = boardTime(sendMsg,incH,incM);
    		sendMsg.led=LED_1; //led 1
	}

	//changing time mode
        if(!strncmp(switchB,"1000",4))  
        {
		printf("time change\n");
            if(*changeFlag==0)   {*changeFlag=1;   sendMsg.led=LED_3_4;} //led 3,4 
            else                {*changeFlag=0;   sendMsg.led=LED_1;}  //led 1
        }   
	//reset time
        if(!strncmp(switchB,"0100",4)) 
        {   
		printf("reset\n");
		incH=0, incM=0;
            sendMsg=boardTime(sendMsg,incH,incM);
        }   
	//increase one hour
        if(!strncmp(switchB,"0010",4)) 
        {
		printf("inchour\n");
		//check whether changing mode
            	if(*changeFlag)
                {
					sendMsg=incHour(sendMsg);
					incH++;
				}
        }   
	//increase one minute
        if(!strncmp(switchB,"0001",4)) 
        {
		printf("incmin\n");
            if(*changeFlag)
            {
			    sendMsg=incMin(sendMsg);
	    		incM++;
			}    
		}
	
	
	if(!strncmp(switchB,"0000",4))
		boardTime(sendMsg,incH,incM);
	//led flash 
        if(*changeFlag)  
            {
                
                 timerF = 0;
                 timerF = time(NULL);
                 gap=(timerF-*timerS);
               //change led state every per 1 second 
                if(gap >= 1.0)
                {
                    sendMsg=ledClock(sendMsg);   
                    *timerS=timerF;    
                }
            }
            
	
	sendMsg = boardTime(sendMsg,incH,incM);
     	printf("fnd : %d %d %d %d\n",sendMsg.fnd[0],sendMsg.fnd[1],sendMsg.fnd[2],sendMsg.fnd[3]);
	return sendMsg;
}

//with time_t module, it reads current board time
//it reflects user's changing time;
SEND_MSG boardTime(SEND_MSG sendMsg,int incH, int incM)
{
    time_t timer;
    struct tm *t;
  
	//get current time
    timer = time(NULL); 
    t = localtime(&timer); 

//current time
    sendMsg.fnd[0]=(incH+(t->tm_hour))%24/10;
    sendMsg.fnd[1]=(incH+(t->tm_hour))%24%10;
    sendMsg.fnd[2]=(incM+(t->tm_min))%60/10;
    sendMsg.fnd[3]=(incM+(t->tm_min))%60%10;

    return sendMsg;
}

//increase hour following user's input
SEND_MSG incHour(SEND_MSG sendMsg)
{  
    char hourStr[3];
    int hour;

    memset(hourStr,0x00,sizeof(hourStr));
    memcpy(hourStr,sendMsg.fnd,2);
    
	//change from string to decimal and increase one hour
    hour = hourStr[0]*10 + hourStr[1];
    hour = (hour+1)%24;

	//change to string
    memset(hourStr,0x00,sizeof(hourStr));
	hourStr[0]=hour/10;
	hourStr[1]=hour%10;		

    memcpy(sendMsg.fnd,hourStr,2);
    printf("z%s\n",hourStr);
    return sendMsg;
}

//increase minute following user's input
SEND_MSG incMin(SEND_MSG sendMsg)
{
    char minStr[3];
    int min;

	//copy to minStr
    memset(minStr,0x00,sizeof(minStr));
    memcpy(minStr,&(sendMsg.fnd[2]),2);

	//change to decimal and increase one minute
    min = minStr[0]*10 + minStr[1];
    min = (min+1)%60;

	//change to string
    memset(minStr,0x00,sizeof(minStr));
	minStr[0]=min/10;
	minStr[1]=min%10;		


    memcpy(&(sendMsg.fnd[2]),minStr,2);
    return sendMsg;

}

//it change the led state following one second
//twinkle led 
SEND_MSG ledClock(SEND_MSG sendMsg)
{
	//set led
    if (sendMsg.led==LED_3) //led is 4
        sendMsg.led=LED_4; //change to led :0010
    else if(sendMsg.led==LED_4 || sendMsg.led==LED_3_4) 
        sendMsg.led=LED_3; //0001
    
    printf("led : %d\n",sendMsg.led);
    return sendMsg;
}
