#define DECIMAL_NUMB 10
#define OCTAL_NUMB 8
#define QUARTER_NUMB 4
#define BINARY_NUMB 2


typedef struct{
//	char device;
	char fnd[4];
	unsigned char led;
	char buf[250];
}SEND_MSG;

typedef struct {
	char device;
	unsigned char readKey;
	char switchB[9];
	char buf[244];
//	char msgBuf[254];
}REV_MSG;

SEND_MSG clockMode(SEND_MSG sendMsg, REV_MSG revMsg,char* changeFlag, time_t* timeStart);
SEND_MSG counterMode(SEND_MSG sendMsg, REV_MSG revMsg, char* numbState);
