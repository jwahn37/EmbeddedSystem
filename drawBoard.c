#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "modules.h"

SEND_MSG reset(SEND_MSG sendMsg, unsigned char (*board)[10], unsigned char (*curLoc)[2], unsigned char *curFlag);
SEND_MSG cursor(SEND_MSG sendMsg, unsigned char* curFlag);
SEND_MSG clear(SEND_MSG sendMsg, unsigned char (*board)[10]);
SEND_MSG reversal(SEND_MSG sendMsg, unsigned char (*board)[10]);
SEND_MSG moveCursor(SEND_MSG sendMsg, REV_MSG revMsg, unsigned char (*board)[10], unsigned char (*curLoc)[2], unsigned char curFlag);
SEND_MSG incFnd(SEND_MSG);

int checkEmpty(char curLoc[2], unsigned char board[10]);


SEND_MSG drawBoard(SEND_MSG sendMsg, REV_MSG revMsg, time_t* timerS)
{
	static unsigned char board[10];
	static unsigned char curLoc[2];	//current location curLoc[0] means height of board, curLoc[1] means width of board
	static unsigned char curFlag=0;
	static unsigned flashFlag=1;	
	int i;
	char initSwitch[4], clickedSwitch[9];
	time_t timerF;
	int gap;
	//check init
	memcpy(initSwitch,"9999",4);
	for(i=0;i<4;i++)	initSwitch[i]-='0';
	if(!memcmp(revMsg.switchB,initSwitch,4))
	{
		memset(board,0x00,10);
		curLoc[0]=0;
		curLoc[1]=64;
		curFlag=0;

	}

	if(revMsg.switchB[0]==1)
		sendMsg=reset(sendMsg, &board, &curLoc, &curFlag);
	else if(revMsg.switchB[2]==1)	//cursor
		sendMsg = cursor(sendMsg, &curFlag);
	else if(revMsg.switchB[6]==1)	//clear
		sendMsg=clear(sendMsg, &board);
	else if(revMsg.switchB[8]==1)	//reversal
		sendMsg=reversal(sendMsg, &board);
	else				//move cursor
		sendMsg=moveCursor(sendMsg, revMsg, &board, &curLoc, curFlag);

	//fnd increase
	memcpy(clickedSwitch,"000000000",9);
	for(i=0;i<10;i++)	clickedSwitch[i]-='0';
	if(memcmp(revMsg.switchB,clickedSwitch,9) && memcmp(revMsg.switchB,initSwitch,4)) //switch was clicked
	{
		sendMsg=incFnd(sendMsg);
	}
	
	
	//flash
             //   ledClock(sendMsg,timer);
        timerF = 0;
        timerF = time(NULL);
        gap=(timerF-*timerS);
	printf("flash : %d %d %d\n",timerF,*timerS,gap);
                //gap is one second change led
        if(gap >= 1.0)
        {        
		if(curFlag==0 && flashFlag==1) 
		{
		printf("hello\n");	
		flashFlag=0;
		}
		else if(curFlag==0 && flashFlag==0)
		{
			if(checkEmpty(curLoc, sendMsg.dot) )
				sendMsg.dot[curLoc[0]] += curLoc[1];
			flashFlag=1;
		}
                 //   sendMsg=ledClock(sendMsg);   
        	*timerS=timerF;    
        }
            



	return sendMsg;
}

SEND_MSG reset(SEND_MSG sendMsg, unsigned char (*board)[10], unsigned char (*curLoc)[2], unsigned char* curFlag)
{
	sendMsg = clear(sendMsg, board);
	(*curLoc)[0]=0,	(*curLoc)[1]=64;
	*curFlag=0;
	
	return sendMsg;
}

SEND_MSG cursor(SEND_MSG sendMsg, unsigned char* curFlag)
{
	*curFlag = (*curFlag == 0) ? 1 : 0;
	return sendMsg;
}

SEND_MSG clear(SEND_MSG sendMsg, unsigned char (*board)[10])
{
	memset(*board,0x00,10);
	memset(sendMsg.dot,0x00,10);	
	return sendMsg;		
}

SEND_MSG reversal(SEND_MSG sendMsg, unsigned char (*board)[10])
{
	int i;
	for(i=0;i<10;i++)
		(*board)[i]=(unsigned char)~((*board)[i]); 
	memcpy(sendMsg.dot,*board,10);
	return sendMsg;
}	


SEND_MSG moveCursor(SEND_MSG sendMsg, REV_MSG revMsg, unsigned char (*board)[10], unsigned char (*curLoc)[2], unsigned char curFlag)
{
	char checkCursor;
	int i;

	if (revMsg.switchB[1]==1)	//up
	{
		(*curLoc)[0] = (*curLoc)[0] - ( ((*curLoc)[0]==1) ? 0 : 1);
	}
	else if(revMsg.switchB[7]==1)	//down
	{
		(*curLoc)[0] = (*curLoc)[0] + ( ((*curLoc)[0]==9) ? 0 : 1); 
	}
	else if(revMsg.switchB[3]==1)	//left
	{
		(*curLoc)[1] = ((*curLoc)[1] == 64) ? (*curLoc)[1] : ( ((*curLoc)[1]==0) ? 1 :(*curLoc)[1] << 1);
	}
	else if(revMsg.switchB[5]==1)	//right
	{
		(*curLoc)[1] = (*curLoc)[1]>>1;
	}
	else if(revMsg.switchB[4]==1)	//select
	{
		if(checkEmpty(*curLoc, *board))
			(*board)[(*curLoc)[0]]+=(*curLoc)[1];
	}
	printf("curLoc : %d %d\n",(*curLoc)[0], (*curLoc)[1]);

	//paint stored board
	memcpy(sendMsg.dot,*board,10);
	
//	if(checkEmpty(*curLoc, sendMsg.dot) && curFlag==0)
//		sendMsg.dot[(*curLoc)[0]] += (*curLoc)[1];

	return sendMsg;
}


SEND_MSG incFnd(SEND_MSG sendMsg)
{
	sendMsg.fnd[3]=(sendMsg.fnd[3]+1)%10;
	if(sendMsg.fnd[3]==0)	
	{
		sendMsg.fnd[2]=(sendMsg.fnd[2]+1)%10;
		if(sendMsg.fnd[2]==0)	
		{
			sendMsg.fnd[1]=(sendMsg.fnd[1]+1)%10;
			if(sendMsg.fnd[1]==0)	
				sendMsg.fnd[0]=(sendMsg.fnd[0]+1)%10;
		}
	}
	return sendMsg;

}

//check if location of current cursor in the board is empty
int checkEmpty(char curLoc[2], unsigned char board[10])
{
	char empty;
	char i, checkCursor;



	for(i=1, checkCursor=0;i<=64;i=i*2)
	{	
		if(i==curLoc[1])	break;
		checkCursor++;
	}		
	
//	printf("check empty checkCursor : %d %d\n",checkCursor,board[curLoc[0]]>>checkCursor);
	//locate current cursor if true, the cursor location is empty.
	if((board[curLoc[0]] >> checkCursor) %2 == 0)
		return 1;	//empty
	
	else return 0; //not empty
}
