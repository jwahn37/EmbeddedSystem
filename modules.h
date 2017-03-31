typedef struct{
//	char device;
	char fnd[4];
	unsigned char led;
	char buf[250];
}SEND_MSG;

SEND_MSG clockMode(SEND_MSG sendMsg);