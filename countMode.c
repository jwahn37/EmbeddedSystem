#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "modules.h"


SEND_MSG convertNumber(SEND_MSG sendMsg, char* numbState);
SEND_MSG incHundred(SEND_MSG sendMsg, char numbState);
SEND_MSG incTen(SEND_MSG sendMsg, char numbState);
SEND_MSG incOne(SEND_MSG sendMsg, char numbState);
SEND_MSG initSet(SEND_MSG sendMsg, char* numbState);

SEND_MSG counterMode(SEND_MSG sendMsg, REV_MSG revMsg, char* numbState)
{
//	revMsg revMsg;
//	sendMsg sendMsg;
	char switchB[4];
	int i;
//	char numbState;
//	char currentNumb;

	memset(switchB,0x00,4);
	memcpy(switchB,revMsg.switchB,4);
	for(i=0;i<4;i++)	switchB[i]=(char)(switchB[i]+'0');
	if(!strncmp(switchB,"9999",4))	//init
		sendMsg = initSet(sendMsg,numbState);
	if(!strncmp(switchB,"1000",4))	//switch 1 pushed
		sendMsg = convertNumber(sendMsg,numbState);
	if(!strncmp(switchB,"0100",4))	//switch 2 pushed
		sendMsg = incHundred(sendMsg,*numbState);
	if(!strncmp(switchB,"0010",4))	//switch 3 pushed
		sendMsg = incTen(sendMsg,*numbState);
	if(!strncmp(switchB,"0001",4))	//switch 4 pushed
		sendMsg = incOne(sendMsg,*numbState);

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

SEND_MSG initSet(SEND_MSG sendMsg, char* numbState)
{

	*numbState=DECIMAL_NUMB;

	sendMsg.fnd[0]=0;
	sendMsg.fnd[1]=0;
	sendMsg.fnd[2]=0;
	sendMsg.fnd[3]=0;
	return sendMsg;
}
SEND_MSG convertNumber(SEND_MSG sendMsg,char* numbState)
{
	char curNum[4];
//	char newNum[4];
	int newNum;
	memset(curNum,0x00,4);
	memcpy(curNum,sendMsg.fnd,4);


//	printf("cur Num : [%d][%d][%d][%d] numb state : %d\n",curNum[0],curNum[1],curNum[2],curNum[3],*numbState);	
	if(*numbState==BINARY_NUMB)//binary -> decimal
	{
		newNum=curNum[0]*8+curNum[1]*4+curNum[2]*2+curNum[3]*1;	
		
		sendMsg.fnd[0]=(newNum/1000);
		sendMsg.fnd[1]=(newNum/100)%10;
		sendMsg.fnd[2]=(newNum/10)%10;
		sendMsg.fnd[3]=newNum%10;
	
		*numbState=DECIMAL_NUMB;	
		printf("ggggggggggggggggggggggggggggggggggggggggggggg");
	
	}
	else if(*numbState==DECIMAL_NUMB) //decimal -> octal
	{
		newNum=curNum[0]*1000+curNum[1]*100+curNum[2]*10+curNum[3]*1;
		
	//	newNum=convDecToOct(newNum);	
		sendMsg.fnd[3]=newNum%8;
		newNum /= 8;
		sendMsg.fnd[2]=newNum%8;
		newNum /= 8;
		sendMsg.fnd[1]=newNum%8;
		newNum /= 8;
		sendMsg.fnd[0]=newNum%8;
	
		*numbState=OCTAL_NUMB;
		printf("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
	}	
	else if(*numbState==OCTAL_NUMB) //octal->quarter
	{
		newNum=curNum[0]*8*8*8+curNum[1]*8*8+curNum[2]*8+curNum[3]*1;
		
	/*	sendMsg.fnd[0]=((newNum/10)%10)/4;
		sendMsg.fnd[1]=((newNum/10)%10)%4;
		sendMsg.fnd[2]=(newNum%10)/4;
		sendMsg.fnd[3]=(newNum%10)%4;
	*/	
		
		sendMsg.fnd[3]=newNum%4;
		newNum /= 4;
		sendMsg.fnd[2]=newNum%4;
		newNum /= 4;
		sendMsg.fnd[1]=newNum%4;
		newNum /= 4;
		sendMsg.fnd[0]=newNum%4;
	
		*numbState=QUARTER_NUMB;
	}
	else if(*numbState==QUARTER_NUMB)//quarter->binary
	{
		
		newNum=curNum[0]*1000+curNum[1]*100+curNum[2]*10+curNum[3]*1;

		sendMsg.fnd[0]=((newNum/10)%10)/2;
		sendMsg.fnd[1]=((newNum/10)%10)%2;
		sendMsg.fnd[2]=(newNum%10)/2;
		sendMsg.fnd[3]=(newNum%10)%2;
		
		*numbState=BINARY_NUMB;
	}

//	printf("numb state : %d\n",*numbState);	

	printf("cur Num : [%d][%d][%d][%d] numb state : %d\n",curNum[0],curNum[1],curNum[2],curNum[3],*numbState);	
	return sendMsg;
}

SEND_MSG incHundred(SEND_MSG sendMsg,char numbState)
{
	sendMsg.fnd[1]=(sendMsg.fnd[1]+1)%numbState;
	if(sendMsg.fnd[1]==0)
		sendMsg.fnd[0]=(sendMsg.fnd[0]+1)%numbState;
	return sendMsg;
}
SEND_MSG incTen(SEND_MSG sendMsg, char numbState)
{
	sendMsg.fnd[2]=(sendMsg.fnd[2]+1)%numbState;
	if(sendMsg.fnd[2]==0)
	{
		sendMsg.fnd[1]=(sendMsg.fnd[1]+1)%numbState;
		if(sendMsg.fnd[1]==0)
			sendMsg.fnd[0]=(sendMsg.fnd[0]+1)%numbState;
	}
	return sendMsg;
}
SEND_MSG incOne(SEND_MSG sendMsg, char numbState)
{
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
