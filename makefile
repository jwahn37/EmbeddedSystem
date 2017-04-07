.SUFFIXES : .c .o 

SRCI =inputProcess.c
OBJI =inputProcess
SRCO =outputProcess.c
OBJO =outputProcess

OBJM =clockMode.o
SRCM =clockMode.c

OBJM2 = countMode.o
SRCM2 = countMode.c

OBJM3 = textEditor.o
SRCM3 = textEditor.c

OBJM4 = drawBoard.o
SRCM4 = drawBoard.c

OBJM5 = calculator.o
OBJM5 = calculator.c

SRC = mainProcess.c
CC = arm-none-linux-gnueabi-gcc -static  
CFLAGS = -g -c 
TARGET =20121604

$(TARGET) : $(OBJO)
	$(CC) -o $(TARGET) $(SRC) $(OBJM) $(OBJM2) $(OBJM3) $(OBJM4) $(OBJM5)
	$(SEND) $(EXECDIR) $(BOPATH)	

$(OBJO) : $(OBJI)
	$(CC) -o $(OBJO) $(SRCO)

$(OBJI) :$(OBJM)
	$(CC) -o $(OBJI) $(SRCI)

$(OBJM) :$(OBJM2)
	$(CC) -c $(SRCM)
$(OBJM2) :$(OBJM3)
	$(CC) -c $(SRCM2)
$(OBJM3) :$(OBJM4)
	$(CC) -c $(SRCM3)
$(OBJM4) :$(OBJM5)
	$(CC) -c $(SRCM4)
%(OBJM5) :
	$(CC) -c $(SRCM5)

clean : 
	rm -rf $(TARGET)
	rm -rf $(OBJO)
	rm -rf $(OBJI)
	rm -rf $(OBJM)
	rm -rf $(OBJM2)
	rm -rf $(OBJM3)
	rm -rf $(OBJM4)
