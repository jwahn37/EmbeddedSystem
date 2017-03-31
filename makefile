.SUFFIXES : .c .o 

EXECDIR =./ExecDir/
SRCI =inputProcess.c
OBJI =$(EXECDIR)inputProcess
SRCO =outputProcess.c
OBJO =$(EXECDIR)outputProcess

OBJM =clockMode.o
SRCM =clockMode.c

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
	$(CC) -o $(TARGET) $(SRC) $(OBJM) 
	$(SEND) $(EXECDIR) $(BOPATH)	
$(OBJO) : $(OBJI)
	$(CC) -o $(OBJO) $(SRCO)

$(OBJI) :$(OBJM)
	$(CC) -o $(OBJI) $(SRCI)

$(OBJM) :
	$(CC) -c $(SRCM)

clean : 
	rm -rf $(TARGET)
	rm -rf $(OBJO)
	rm -rf $(OBJI)
	rm -rf $(PIPEI)
	rm -rf $(PIPEO)
	rm -rf $(OBJM)
