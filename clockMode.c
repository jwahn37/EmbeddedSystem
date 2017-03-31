#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define LED_1 128
#define LED_3_4 48
#define LED_3 32
#define LED_4 16

void outputProcess();
void mainProcess();
void inputProcess();



typedef struct{
	char fnd[4];	//size = 4
	unsigned char led;
	char revBuf[250];
}SEND_MSG;

typedef struct{
	char device;
	char readKey[2];
	char switchB[9];
	char buf[243];

}REV_MSG;

SEND_MSG incHour(SEND_MSG sendMsg);
SEND_MSG incMin(SEND_MSG sendMsg);
SEND_MSG ledClock(SEND_MSG sendMsg);
SEND_MSG boardTime(SEND_MSG sendMsg);
int main(void) {

  time_t timerS=0,timerF=0;
  float gap;
 // struct tm *t;
  char switchP[4];
  char changeFlag=0;
  char input;
  REV_MSG revMsg;
  SEND_MSG sendMsg;
 //fnd 
 /*timer = time(NULL); // 현재 시각을 초 단위로 얻기

  t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기

//최초 시간
    sendMsg.fnd[0]=t->tm_hour/10;
    sendMsg.fnd[1]=t->tm_hour%10;
    sendMsg.fnd[2]=t->tm_min/10;
    sendMsg.fnd[3]=t->tm_min%10;
    */
    sendMsg = boardTime(sendMsg);
//최초 led
    sendMsg.led=LED_1; //led 1

    //printf("%d %d",t->tm_min,sendMsg.fnd[3]);

//input Process로부터 입력을 받아온다. sw1,sw2,sw3,sw4
   // srand((unsigned int)time(NULL));
   // timerS = clock(); // 현재 시각을 초 단위로 얻기
 	 timerS = time(NULL);
     while(1){
          //      printf("time : %d %d : %d %d\n",sendMsg.fnd[0],sendMsg.fnd[1],sendMsg.fnd[2],sendMsg.fnd[3]);
                changeFlag=1;
        
       
        //printf("%ld\n",time(NULL));

	    memcpy(switchP,revMsg.switchB,4);

        if(!strcmp(switchP,"1000"))  //시간 변화 가능
        {
            if(changeFlag==0)   {changeFlag=1;   sendMsg.led=LED_3_4;} //led 3,4 
            else                {changeFlag=0;   sendMsg.led=LED_1;}  //led 1
        }   
        if(!strcmp(switchP,"0100")) //시간 reset
        {   
            boardTime(sendMsg);
        }   
        if(!strcmp(switchP,"0010")) //한시간증가
        {
            if(changeFlag)
                sendMsg=incHour(sendMsg);
        }   
        if(!strcmp(switchP,"0001")) //1분증가
        {
            if(changeFlag)
                sendMsg=incMin(sendMsg);
        }
        if(changeFlag)  //led
            {
                
             //   ledClock(sendMsg,timer);
                 timerF = 0;
                 timerF = time(NULL);
                gap=(timerF-timerS);
                
                //printf("%f\n",gap); //this should not be erased because ...
                //gap is one second change led
                if(gap >= 1.0)
                {
                    
                    sendMsg=ledClock(sendMsg);
                    
                    timerS=timerF;    
                }
            }
            
	}   
     return 0;
}

SEND_MSG boardTime(SEND_MSG sendMsg)
{
    time_t timer;
    struct tm *t;
  
    timer = time(NULL); // 현재 시각을 초 단위로 얻기

    t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기

//최초 시간
    sendMsg.fnd[0]=t->tm_hour/10;
    sendMsg.fnd[1]=t->tm_hour%10;
    sendMsg.fnd[2]=t->tm_min/10;
    sendMsg.fnd[3]=t->tm_min%10;

    return sendMsg;
}

SEND_MSG incHour(SEND_MSG sendMsg)
{  
    //SEND_MSG retMsg;
    char hourStr[3];
    int hour;

    memset(hourStr,0x00,sizeof(hourStr));
    memcpy(hourStr,sendMsg.fnd,2);
    
  //  printf("z%s\n",hourStr);

   hour = hourStr[0]*10 + hourStr[1];
   hour = (hour+1)%24;

    memset(hourStr,0x00,sizeof(hourStr));
 //   itoa(hour, hourStr ,10);
	hourStr[0]=hour/10;
	hourStr[1]=hour%10;		
		

    memcpy(sendMsg.fnd,hourStr,2);
    printf("z%s\n",hourStr);
    return sendMsg;
}
SEND_MSG incMin(SEND_MSG sendMsg)
{
    //SEND_MSG sendMsg;
    char minStr[3];
    int min;

    memset(minStr,0x00,sizeof(minStr));
    memcpy(minStr,&(sendMsg.fnd[2]),2);

    min = minStr[0]*10 + minStr[1];
    min = (min+1)%60;

    memset(minStr,0x00,sizeof(minStr));
//    itoa(min, minStr ,10);
	minStr[0]=min/10;
	minStr[1]=min%10;		


    memcpy(&(sendMsg.fnd[2]),minStr,2);


    return sendMsg;

}

SEND_MSG ledClock(SEND_MSG sendMsg)
{
    if (sendMsg.led==LED_3) //led is 4
        sendMsg.led=LED_4; //change to led :0010
    else if(sendMsg.led==LED_4 || sendMsg.led==LED_3_4) 
        sendMsg.led=LED_3; //0001
    
    printf("led : %d\n",sendMsg.led);
    return sendMsg;
}
