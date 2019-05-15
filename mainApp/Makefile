
#cdir:=mainApp
target:=main
src:= ./mainApp/mainApp.c ./libipc/ipc.c ./libstore/store.c ./protocol/protocol.c

$(target): $(src)
	@echo "build mainApp"
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

