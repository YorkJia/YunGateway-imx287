CROSS = arm-fsl-linux-gnueabi-

CC = $(CROSS)gcc

STRIP = $(CROSS)strip 

CFLAGS = -g -O2 -Wall

#include

INC = -I./bsp -I./bsp/wraperror \
       -I./bsp/wrappthread \
       -I./bsp/driver/mb_rtu \
       -I./lib/libmodbus \
       -I./bsp/event
#lib
LIBS = -L./lib/libmodbus -lmodbus -lpthread

#src
SRC = main.c  bsp/wraperror/wraperror.c \
      bsp/wrappthread/wrappthread.c \
      bsp/driver/mb_rtu/mb_rtu.c \
      bsp/event/portevent.c 

#target
TARGET = test

#objs
OBJS = $(SRC:.c=.o)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJS)

install: $(TARGET) clean
	@echo start compile...
	@echo end.

%.o:%.c
	$(CC) $(CFLAGS) $(INC) $(LIBS) -o $@ -c $<



