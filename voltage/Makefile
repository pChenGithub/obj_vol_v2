
cdir:=voltage
target:=vol
src:= ./voltage/vol.c ./libipc/ipc.c ./libuart/uart.c

#all: $(target)
#	@echo "123132"

$(target): $(src)
	@echo "build vol"
	$(CC) $^ -o $@ $(CFLAGS)

