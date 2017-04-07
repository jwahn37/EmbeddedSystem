#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "modules.h"
#define NUM_BUTTON 9
#define STR_LEN 16

SEND_MSG inputCalc(SEND_MSG sendMsg, REV_MSG revMsg);
SEND_MSG outputCalc(SEND_MSG sendMsg, REV_MSG revMsg, char input[16], char (*output)[16]);
//SEND_MGS keepErr(SEND_MSG sendMsg, REV_MSG revMsg, char pastMsg[32]);
static int numOfInput(REV_MSG revMsg);
char* changeToStr(float ans);
int checkErr(char input[16], char oper);
SEND_MSG calculator(SEND_MSG sendMsg, REV_MSG revMsg)
{
	char init[4];
	int i;	
	static char pastMsg[32];
/*	//init
	memcpy(init,"9999",4);
	for(i=0;i<4;i++)	init-='0';
	if(!memcmp(sendMsg.switchB,init,4))
	{
		for(i=0;i<STR_LEN;i++)
		{
			input[i]=0;
			output[i]=0;
		}

	}
*/	
	sendMsg=inputCalc(sendMsg, revMsg);
	memcpy(pastMsg,sendMsg.lcd,32);
//	sendMsg=keepErr(sendMsg,revMsg,pastMsg);
	return sendMsg;
}
/*
SEND_MSG keepErr(sendMsg,revMsg,pastMsg)
{
	



}
*/
static int numOfInput(REV_MSG revMsg)
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

SEND_MSG inputCalc(SEND_MSG sendMsg, REV_MSG revMsg)
{
	static char input[16];
	static char output[16];
	int idx=0;
	int jdx=0;
	int clickedTwo[2];
	char oper;
	char init[4];
	
	//init
	memcpy(init,"9999",4);
	for(idx=0;idx<4;idx++)	init[idx]-='0';
	if(!memcmp(revMsg.switchB,init,4))
		memset(input,0x00,16);
	
	memset(&sendMsg.lcd[16],0x00,16);

	if(numOfInput(revMsg)!=0)	//new calculation
	{
		printf("hello\n");
		if(output[0]!=0)
		{
			memset(output,0x00,16);
			memset(input,0x00,16);
		}
	}	

	if(numOfInput(revMsg)==1)	//one button clicked
	{
		//the clicked switch number
		for(idx=0;idx<NUM_BUTTON;idx++)
		{
			if(revMsg.switchB[idx]==1)
				break;
		}	
		
		
		input[strlen(input)]=idx+'1';
	}
	
	else if(numOfInput(revMsg)==2)	//two button clicked
	{
		for(idx=0,jdx=0; idx<NUM_BUTTON;idx++)
		{
			if(revMsg.switchB[idx]==1)
			{
				clickedTwo[jdx]=idx;		
				jdx++;
			}
		}
		printf("clicked two : %d %d\n",clickedTwo[0],clickedTwo[1]);

		if(clickedTwo[0]==0 && clickedTwo[1]==1) //operation '+'
			oper='+';
		if(clickedTwo[0]==1 && clickedTwo[1]==2) //operation '-'
			oper='-';
		if(clickedTwo[0]==3 && clickedTwo[1]==4) //operation '*'
			oper='*';
		if(clickedTwo[0]==4 && clickedTwo[1]==5) //operation '/'
			oper='/';
		if(clickedTwo[0]==6 && clickedTwo[1]==7) //number 0
			oper='0';
		if(clickedTwo[0]==7 && clickedTwo[1]==8) //enter 
		{
			oper=0;
			sendMsg=outputCalc(sendMsg,revMsg,input,&output);	
		}
		if(!checkErr(input,oper))
			input[strlen(input)]=oper;	

	}

	memcpy(sendMsg.lcd,input,16);
	memcpy(&sendMsg.lcd[16],output,16);
	return sendMsg;
}

int checkErr(char input[16], char oper)
{
	int i;
	int numOper;
	for(i=0,numOper=0;i<16;i++)
	{
		if(input[i]=='+' || input[i]=='-' || input[i]=='*' || input[i]=='/')	
			numOper++;
	}

	if(numOper==1)	//operation already exists
		return 1;
	if(strlen(input)>=16) //to many input 
		return 1;
	
	if(strlen(input)==0)
		return 1;
	else 
		return 0;
}

SEND_MSG outputCalc(SEND_MSG sendMsg, REV_MSG revMsg, char input[16], char (*output)[16])
{
//	static char output[16];
	int i,j;
	float ans;
	float val[2];	//input value
	char operation;
	char init[4];

	//init
	printf("outputCalc\n");
	memcpy(init,"9999",4);
	for(i=0;i<4;i++)	init[i]-='0';
	if(!memcmp(revMsg.switchB,init,4))
		memset(*output,0x00,16);
	
	for(i=0,j=0;i<STR_LEN;i++)
	{
		if(input[i]>='0' && input[i]<='9')	//input is number
			val[j]=val[j]*10+(input[i]-'0');
	
		if(input[i]=='+' || input[i]=='-' || input[i]=='*' || input[i]=='/')	//input is operation;
		{
			operation=input[i];
			j++;
		}	

	}
	
	if(operation=='+')	ans=val[0]+val[1];
	if(operation=='-')	ans=val[0]-val[1];
	if(operation=='*')	ans=val[0]*val[1];
	if(operation=='/')	ans=val[0]/val[1];

	printf("%f %f %c\n",val[0],val[1],operation);

	memcpy(*output,changeToStr(ans),16);
	memcpy(&sendMsg.lcd[16],*output,16);
	
	return sendMsg;
}

char* changeToStr(float ans)
{
	static char str[16];
	
	memset(str,0x00,16);
	sprintf(str, "%f", ans);	
	return str;
}
