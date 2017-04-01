#include<stdio.h>
#include<stdlib.h>
#include "modules.h"

#define DECIMAL_NUMB 10
#define OCTAL_NUMB 8
#define QUARTER_NUMB 4
#define BINARY_NUMB 2

SEND_MSG convertNumber(SEND_MSG sendMsg);
SEND_MSG incHundred(SEND_MSG sendMsg);
SEND_MSG incTen(SEND_MSG sendMsg);
SEND_MSG incOne(SEND_MSG sendMsg);


int main()
{
	revMsg revMsg;
	sendMsg sendMsg;
	char switchB[4];
	char numbState;
	memset(switchB,0x00,4);
	memcpy(switchB,revMsg.switchB,4);
	
	if(!strncmp(switchB,"1000",4))	//switch 1 pushed
		sendMsg = converNumber(sendMsg,&numbState);
	if(!strncmp(switchB,"0100",4))	//switch 2 pushed
		sendMsg = incHundred(sendMsg);
	if(!strncmp(switchB,"0010",4))	//switch 3 pushed
		sendMsg = incTen(sendMsg);
	if(!strncmp(switchB,"0001",4))	//switch 4 pushed
		sendMssg = incOne(sendMsg);

	printf("%d %d %d %d \n",sendMsg.fnd[0], sendMsg.fnd[1], sendMsg.fnd[2], sendMsg.fnd[3]);
	return 0;
}


SEND_MSG convertNumber(SEND_MSG sendMsg,char* numbState)
{
	char curNum[4];
//	char newNum[4];
	int numNum;
	memset(curNum,0x00,4);
	memcpy(curNum,sendMsg.fnd,4);

	if(*numbState==DECIMAL_NUMB)//binary -> decimal
	{
		newNum=curNum[0]*8+curNum[1]*4+curNum[2]*2+curNum[3]*1;	
		
		sendMsg.fnd[0]=(numNum/1000);
		sendMsg.fnd[1]=(numNum/100)%10;
		sendMsg.fnd[2]=(numNum/10)%10;
		sendMsg.fnd[3]=newNum%10;
	
		*numbState=DECIMAL_NUMB;	
	}
	else if(numbState==OCTAL_NUMB) //decimal -> octal
	{
		newNum=curNum[0]*1000+curNum[1]*100+curNum[2]*10+curNum[3]*1;
	//	newNum=convDecToOct(newNum);	
		sendMsg.fnd[3]=newNum%8;
		newNum/=8;
		sendMsg.fnd[2]=newNum%8;
		newNum/=8;
		sendMsg.fnd[1]=newNum%8;
		revNum/=8;
		sendMsg.fnd[0]=newNum%8;
	}	
	else if(numbState==QUARTER_NUMB) //octal->quarter
	{
		newNum=curNum[0]*1000+curNum[1]*100+curNum[2]*10+curNum[3]*1;
		
		sendMsg[0]=((newNum/10)%10)/4;
		sendMsg[1]=((newNum/10)%10)%4;
		sendMsg[2]=(newNum%10)/4;
		sendMsg[3]=(newNum%10)%4;

	}
	else if(numbState==BINARY_NUMB)//quarter->binary
	{
		
		newNum=curNum[0]*1000+curNum[1]*100+curNum[2]*10+curNum[3]*1;

		sendMsg[0]=((newNum/10)%10)/2;
		sendMsg[1]=((newNum/10)%10)%2;
		sendMsg[2]=(newNum%10)/2;
		sendMsg[3]=(newNum%10)%2;
	}

	
	return sendMsg;
}

SEND_MSG incHundred(SEND_MSG sendMsg);
SEND_MSG incTen(SEND_MSG sendMsg);
SEND_MSG incOne(SEND_MSG sendMsg);
