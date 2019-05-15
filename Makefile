
CROSS_COMPILE?=arm-fsl-linux-gnueabi-
#CROSS_COMPILE?=
CC:=$(CROSS_COMPILE)gcc
INC:=-I./include
CFLAGS:= $(INC) -DDEBUG_P -DDEBUG
LDFLAGS:= -lpthread



.PHONY : all
all: main vol readCard transferBle
	@echo "build all"


#target:=test
#src:= test.c libipc/ipc.c

#$(target): $(src)
#	@echo "build test"
#	$(CC) $^ -o $@ $(CFLAGS)

include ./mainApp/Makefile
include ./voltage/Makefile
include ./rfidCard/Makefile
include ./transfer/Makefile

.PHONY : clean
	rm 


