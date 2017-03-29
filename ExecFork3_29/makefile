.SUFFIXES : .c .o 

SRCI = inputProcess.c
OBJI = inputProcess
SRCO = outputProcess.c
OBJO = outputProcess

PIPEI = input_pipeLine
PIPEO = output_pipeLine

SRC = execPipe.c
CC = gcc 
CFLAGS = -g -c 
TARGET = 20121604

$(TARGET) : $(OBJO)
	$(CC) -o $(TARGET) $(SRC) 
	
$(OBJO) : $(OBJI)
	$(CC) -o $(OBJO) $(SRCO)

$(OBJI) : 
	$(CC) -o $(OBJI) $(SRCI)


clean : 
	rm -rf $(TARGET)
	rm -rf $(OBJO)
	rm -rf $(OBJI)
	rm -rf $(PIPEI)
	rm -rf $(PIPEO)

