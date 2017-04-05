#include<stdio.h>
#include "module.h"

#define NUM_BUTTON 9

int numOfInput(REV_MSG revMsg);


//SEND_MSG textEditor(SEND_MSG sendMsg, REV_MSG revMsg)
int main()
{
	SEND_MSG sendMsg, REV_MSG revMsg;
	char switchBuf[3];
	char alphabet[9][3]={ {'.','Q','Z'}, {'A','B','C'}, {'D','E','F'}, {'G','H','I'}, {'J','K','L'}, {'M','N','O'}, {'P','R','S'}, {'T','U','V'}, {'W','X','Y'}};

	int nOfIp = nOfIp = numOfInput(revMsg);
	int i;

	if(nOfIp==1)	//only one switch occured
	{
		for(i=0;i<NUM_BUTTON ; i++)				
		{

	}
}


int numOfInput(REV_MSG revMsg)
{
	int i;
	int nOfIp=0;
	for(i=0;i<NUM_BUTTON;i++)
		nOfIp+=1;
	return nOfIp;

}
