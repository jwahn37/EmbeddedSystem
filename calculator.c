#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUM_BUTTON 9

void inputCalc(REV_MSG revMsg, char (*input)[16]);
SEND_MSG output(SEND_MSG sendMsg, char input[16]);
int numOfInput(REV_MSG revMsg);

SEND_MSG calculator(SEND_MSG sendMsg, REV_MSG revMsg)
{
	static char input[16];
	static char output[16];
	char init;
	int i;	
	//init
	memcpy(init,"9999",4);
	for(i=0;i<4;i++)	init-='0';
	if(!memcmp(sendMsg.switchB,init,4))
	{
		for(i=0;i<16;i++)
		{
			input[i]=0;
			output[i]=0;
		}

	}
	
	inputCalc(revMsg, &input);
	


	return calculator;
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

void input(REV_MSG revMsg, char (*input)[16])
{
	int idx=0;
	int jdx=0;
	int clickedTwo[2];
	if(numOfInput(revMsg)==1)	//one button clicked
	{
		//the clicked switch number
		for(idx=0;idx<NUM_BUTTON;idx++)
		{
			if(revMsg.switchB[idx]==1)
				break;
		}	
		
		
		(*input)[strlen(*input)]=idx+'1';
	}
	
	else if(numOfInput(revMsg)==2)	//two button clicked
	{
		for(idx=0,jdx=0; idx<NUM_BOTTON;idx++)
		{
			if(revMsg.switchB[idx]==1)
			{
				clickedTwo[j]++;		
				j++;
			}
		}

			

	}




}
