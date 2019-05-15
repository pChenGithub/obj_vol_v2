
cdir:=rfidCard
target:=readCard
src:= ./rfidCard/readCard.c ./libipc/ipc.c ./libuart/uart.c ./librfid/rfid.c

#all: $(target)
#	@echo "123132"

$(target): $(src)
	@echo "build readCard"
	$(CC) $^ -o $@ $(CFLAGS)

