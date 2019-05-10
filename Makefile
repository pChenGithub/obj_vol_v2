
CROSS_COMPILE?=arm-fsl-linux-gnueabi-
CC:=$(CROSS_COMPILE)gcc
INC:=-I./include
CFLAGS:= $(INC)
LDFLAGS:=



.PHONY : all
all: rfid vol

target:=rfid
src:= rfid.c ipc/ipc_msg.c

$(target): $(src)
	$(CC) $^ -o $@ $(CFLAGS)


target:=vol
src:= vol.c ipc/ipc_msg.c

$(target): $(src)
	$(CC) $^ -o $@ $(CFLAGS)

.PHONY : clean
	rm 


