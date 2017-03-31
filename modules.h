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

SEND_MSG clockMode(SEND_MSG sendMsg, REV_MSG revMsg);