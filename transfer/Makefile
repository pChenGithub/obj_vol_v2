
cdir:=transfer
target:=transferBle
src:= ./transfer/transfer.c ./transfer/ble.c ./transfer/pack_and_send.c ./libipc/ipc.c ./libuart/uart.c ./protocol/protocol.c

#all: $(target)
#	@echo "123132"

$(target): $(src)
	@echo "build transferBle"
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

