#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "modules.h"


SEND_MSG convertNumber(SEND_MSG sendMsg, char* numbState, int* count);
SEND_MSG incHundred(SEND_MSG sendMsg, char numbState, int *count);
SEND_MSG incTen(SEND_MSG sendMsg, char numbState, int* count);
SEND_MSG incOne(SEND_MSG sendMsg, char numbState, int* count);
SEND_MSG initSet(SEND_MSG sendMsg, char* numbState, int* count);

//it print current counter and state by LED , FND device. user can change counter
SEND_MSG counterMode(SEND_MSG sendMsg, REV_MSG revMsg, char* numbState)
{
	char switchB[4];
	int i;
	static int count=0;
	
	memset(switchB,0x00,4);
	memcpy(switchB,revMsg.switchB,4);
	for(i=0;i<4;i++)	switchB[i]=(char)(switchB[i]+'0');
	if(!strncmp(switchB,"9999",4))	//init
		sendMsg = initSet(sendMsg,numbState,&count);
	if(!strncmp(switchB,"1000",4))	//switch 1 pushed
		sendMsg = convertNumber(sendMsg,numbState,&count);
	if(!strncmp(switchB,"0100",4))	//switch 2 pushed
		sendMsg = incHundred(sendMsg,*numbState,&count);
	if(!strncmp(switchB,"0010",4))	//switch 3 pushed
		sendMsg = incTen(sendMsg,*numbState,&count);
	if(!strncmp(switchB,"0001",4))	//switch 4 pushed
		sendMsg = incOne(sendMsg,*numbState,&count);

	//decimal can't be more than 1000
	if(*numbState==DECIMAL_NUMB)
		sendMsg.fnd[0]=0;
	//led state
	if(*numbState==DECIMAL_NUMB){	sendMsg.led=64;	}
	if(*numbState==OCTAL_NUMB)  {	sendMsg.led=32;	}
	if(*numbState==QUARTER_NUMB){	sendMsg.led=16;	}
	if(*numbState==BINARY_NUMB) {	sendMsg.led=128;}
	
	printf("from mode 2 : %d %d %d %d , numbState : %d\n",sendMsg.fnd[0], sendMsg.fnd[1], sendMsg.fnd[2], sendMsg.fnd[3],*numbState);
	return sendMsg;
}

//initialize counter to "0000"
SEND_MSG initSet(SEND_MSG sendMsg, char* numbState, int* count)
{

	*numbState=DECIMAL_NUMB;
	*count=0;
	
	sendMsg.fnd[0]=0;
	sendMsg.fnd[1]=0;
	sendMsg.fnd[2]=0;
	sendMsg.fnd[3]=0;
	return sendMsg;
}

//conver the number 
//order : decimal -> octal -> quarter -> binary -> decimal...
SEND_MSG convertNumber(SEND_MSG sendMsg,char* numbState, int* count)
{
	char curNum[4];
	int newNum;
	memset(curNum,0x00,4);
	memcpy(curNum,sendMsg.fnd,4);

	//change binary to decimal
	if(*numbState==BINARY_NUMB)
	{
		sendMsg.fnd[0]=*count/1000;
		sendMsg.fnd[1]=(*count/100)%10;
		sendMsg.fnd[2]=(*count/10)%10;
		sendMsg.fnd[3]=(*count)%10;
		
		*numbState=DECIMAL_NUMB;	
	}
	//change decimal to octal
	else if(*numbState==DECIMAL_NUMB) 
	{
		newNum=*count;

		sendMsg.fnd[3]=newNum%8;
		newNum /= 8;
		sendMsg.fnd[2]=newNum%8;
		newNum /= 8;
		sendMsg.fnd[1]=newNum%8;
		newNum /= 8;
		sendMsg.fnd[0]=newNum%8;
	
		*numbState=OCTAL_NUMB;
	}
	//change octal to quarter	
	else if(*numbState==OCTAL_NUMB) 
	{
		newNum=*count;
	
		sendMsg.fnd[3]=newNum%4;
		newNum /= 4;
		sendMsg.fnd[2]=newNum%4;
		newNum /= 4;
		sendMsg.fnd[1]=newNum%4;
		newNum /= 4;
		sendMsg.fnd[0]=newNum%4;
	
		*numbState=QUARTER_NUMB;
	}
	//change quarter to binary
	else if(*numbState==QUARTER_NUMB)
	{
		newNum=*count;	
		sendMsg.fnd[3]=newNum%2;
		newNum /= 2;
		sendMsg.fnd[2]=newNum%2;
		newNum /= 2;
		sendMsg.fnd[1]=newNum%2;
		newNum /= 2;
		sendMsg.fnd[0]=newNum%2;
	
		*numbState=BINARY_NUMB;
	}

	printf("cur Num : [%d][%d][%d][%d] numb state : %d\n",curNum[0],curNum[1],curNum[2],curNum[3],*numbState);	
	return sendMsg;
}
//increase hundreds place 
SEND_MSG incHundred(SEND_MSG sendMsg,char numbState, int* count)
{
	*count=*count+numbState*numbState;

	sendMsg.fnd[1]=(sendMsg.fnd[1]+1)%numbState;
	if(sendMsg.fnd[1]==0)
		sendMsg.fnd[0]=(sendMsg.fnd[0]+1)%numbState;
	return sendMsg;
}

//increase tens place
SEND_MSG incTen(SEND_MSG sendMsg, char numbState, int* count)
{
	*count=*count+numbState;
	sendMsg.fnd[2]=(sendMsg.fnd[2]+1)%numbState;
	if(sendMsg.fnd[2]==0)
	{
		sendMsg.fnd[1]=(sendMsg.fnd[1]+1)%numbState;
		if(sendMsg.fnd[1]==0)
			sendMsg.fnd[0]=(sendMsg.fnd[0]+1)%numbState;
	}
	return sendMsg;
}

//increase ones place
SEND_MSG incOne(SEND_MSG sendMsg, char numbState, int* count)
{
	*count=*count+1;
	sendMsg.fnd[3]=(sendMsg.fnd[3]+1)%numbState;
	if(sendMsg.fnd[3]==0)
	{
		sendMsg.fnd[2]=(sendMsg.fnd[2]+1)%numbState;
		if(sendMsg.fnd[2]==0)
		{
			sendMsg.fnd[1]=(sendMsg.fnd[1]+1)%numbState;
			if(sendMsg.fnd[1]==0)
				sendMsg.fnd[0]=(sendMsg.fnd[0]+1)%numbState;
		}
	}

	return sendMsg;
}
