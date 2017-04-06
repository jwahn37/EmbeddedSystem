.SUFFIXES : .c .o 

EXECDIR =./ExecDir/
SRCI =inputProcess.c
OBJI =$(EXECDIR)inputProcess
SRCO =outputProcess.c
OBJO =$(EXECDIR)outputProcess

OBJM =clockMode.o
SRCM =clockMode.c

OBJM2 = countMode.o
SRCM2 = countMode.c

OBJM3 = textEditor.o
SRCM3 = textEditor.c

PIPEI =$(EXECDIR)input_pipeLine
PIPEO =$(EXECDIR)output_pipeLine

SEND =adb push 
BOPATH =/sdcard/20121604_1

SRC = mainProcess.c
CC = arm-none-linux-gnueabi-gcc -static  
#CC = gcc
CFLAGS = -g -c 
TARGET =$(EXECDIR)20121604

$(TARGET) : $(OBJO)
	$(CC) -o $(TARGET) $(SRC) $(OBJM) $(OBJM2) $(OBJM3)
	$(SEND) $(EXECDIR) $(BOPATH)	
$(OBJO) : $(OBJI)
	$(CC) -o $(OBJO) $(SRCO)

$(OBJI) :$(OBJM)
	$(CC) -o $(OBJI) $(SRCI)

$(OBJM) :$(OBJM2)
	$(CC) -c $(SRCM)
$(OBJM2) :$(OBJM3)
	$(CC) -c $(SRCM2)
$(OBJM3) :
	$(CC) -c $(SRCM3)

clean : 
	rm -rf $(TARGET)
	rm -rf $(OBJO)
	rm -rf $(OBJI)
	rm -rf $(PIPEI)
	rm -rf $(PIPEO)
	rm -rf $(OBJM)
	rm -rf $(OBJM2)
	rm -rf $(OBJM3)
