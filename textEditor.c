#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "modules.h"

#define NUM_BUTTON 9

int numOfButton(REV_MSG revMsg);
SEND_MSG oneButtonClicked(SEND_MSG sendMsg, REV_MSG revMsg,char alphabet[9][3], char (*pastClicked)[2], char numFlag);
SEND_MSG twoButtonClicked(SEND_MSG sendMsg, REV_MSG revMsg,char alphabet[9][3], char (*pastClicked)[2], char* numFlag, unsigned char fpga_number[2][10]);
SEND_MSG fullLcd(SEND_MSG sendMsg,char input);
//SEND_MSG textEditor(SEND_MSG sendMsg, REV_MSG revMsg)
SEND_MSG textEditor(SEND_MSG sendMsg, REV_MSG revMsg)
{
//	SEND_MSG sendMsg;
//	REV_MSG revMsg;
	static char numFlag=0;
	static char pastClicked[2]={100,0}; //1st index : past clicked button number, 2nd index : the num of continuously clicked same button
	static char alphabet[9][3]={ {'.','Q','Z'}, {'A','B','C'}, {'D','E','F'}, {'G','H','I'}, {'J','K','L'}, {'M','N','O'}, {'P','R','S'}, {'T','U','V'}, {'W','X','Y'}};

	static unsigned char fpga_number[2][10] = {
	{0x1c,0x36,0x63,0x63,0x63,0xff,0xff,0x63,0x63,0x63},
	{0x0c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3f,0x3f}
	};
	char initSwitch[4];
	int i;

	printf("textedit start %d %d %d %d %d/////\n",revMsg.switchB[0],revMsg.switchB[1],revMsg.switchB[2],revMsg.switchB[3],memcmp(revMsg.switchB,"9999",4));

	memcpy(initSwitch,"9999",4);
	for(i=0;i<4;i++)	initSwitch[i]=initSwitch[i]-'0';

	if(!memcmp(revMsg.switchB,initSwitch,4))	//init
	{
		printf("lcd init\n");
		memcpy(sendMsg.dot,fpga_number[0],10);
		memset(sendMsg.lcd,0x00,32);
	}
	if(numOfInput(revMsg)==1)	sendMsg=oneButtonClicked(sendMsg,revMsg,alphabet, &pastClicked, numFlag);
	else if(numOfInput(revMsg)==2)	sendMsg=twoButtonClicked(sendMsg,revMsg, alphabet, &pastClicked, &numFlag, fpga_number);


	return sendMsg;
}


int numOfInput(REV_MSG revMsg)
{
	int i;
	int nOfIp=0;
	for(i=0;i<NUM_BUTTON;i++)
	{
		if(revMsg.switchB[i]==1)
			nOfIp+=1;
	}
	
	return nOfIp;

}


SEND_MSG oneButtonClicked(SEND_MSG sendMsg, REV_MSG revMsg, char alphabet[9][3], char (*pastClicked)[2], char numFlag)
{
	int i;
	char clickedNum;
	char curLCD;
	char lcdLength;

	//clicked button number
	for(i=0;i<NUM_BUTTON;i++)
	{
		if(revMsg.switchB[i]==1)
		{	
			clickedNum=i;
			break;
		}
	}

	lcdLength=strlen(sendMsg.lcd);	
//	printf("clicked num : %d lcd Length : %d\n",clickedNum,lcdLength);

	if(numFlag==0){	//alphabet
	//whether the button was clicked in the past
		if(clickedNum!=(*pastClicked)[0])	//this is first click
		{
			curLCD = alphabet[clickedNum][0];
			(*pastClicked)[0]=clickedNum;
			(*pastClicked)[1]=1; 
//			printf("cur LCD : %d %c\n",curLCD, curLCD);
			if(lcdLength<32)
				sendMsg.lcd[lcdLength]=curLCD;
			else
				sendMsg = fullLcd(sendMsg,curLCD);
//			printf("sendMsg.lcd : %d %c\n",sendMsg.lcd[lcdLength],sendMsg.lcd[lcdLength]);		
		}
		else				//this button was already clicked
		{
			curLCD = alphabet[(int)clickedNum][(int)((*pastClicked)[1])];
			(*pastClicked)[1]=((*pastClicked)[1]+1)%3;
	
			printf("cur LCD : %d %c\n",curLCD, curLCD);
			sendMsg.lcd[lcdLength-1]=curLCD;
		}
	}
	
	else	//number
	{
		sendMsg.lcd[lcdLength]='1'+clickedNum;
	}
	printf("lcd string : %s\n",sendMsg.lcd);
		
	return sendMsg;

}
SEND_MSG twoButtonClicked(SEND_MSG sendMsg, REV_MSG revMsg, char alphabet[9][3], char (*pastClicked)[2], char* numFlag, unsigned char fpga_number[2][10])
{
	char clicked[2];
	int i,j;
	char lcdLength;
	//two clicked button number
	for(i=0,j=0;i<NUM_BUTTON;i++)
	{
		if(revMsg.switchB[i]==1)
			clicked[j++]=i+1;
	}
	printf("clcked : %d %d\n",clicked[0],clicked[1]);
		
	lcdLength=strlen(sendMsg.lcd);
	if(clicked[0]==5 && clicked[1]==6)	//change number/alphabet mode	
	{
		(*pastClicked)[0]=100;
		(*pastClicked)[1]=0;
	
		*numFlag = (*numFlag==0)?1:0;	//change num flag
//		sendMsg.dot = (sendMsg.dot==0)? 1 : 0; //dot setting
		memcpy(sendMsg.dot, fpga_number[*numFlag], 10);
	}
	else if(clicked[0]==2 && clicked[1]==3) //clear lcd
	{
		*numFlag=0;
		(*pastClicked)[0]=100, (*pastClicked)[1]=0;
		memset(sendMsg.lcd,0x00,32);
		
	}
	else if(clicked[0]==8 && clicked[1]==9) //blank to lcd
	{
		if(lcdLength<32)
			sendMsg.lcd[lcdLength]=' ';
		else	
			sendMsg = fullLcd(sendMsg,' ');

	}



	return sendMsg;
}

SEND_MSG fullLcd(SEND_MSG sendMsg, char input)
{
	int i;
	for(i=0;i<31;i++)
	{
		sendMsg.lcd[i]=sendMsg.lcd[i+1];	

	}
	sendMsg.lcd[31]=input;
	return sendMsg;
}
