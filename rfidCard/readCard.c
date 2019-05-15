/*************************************************************************
	> File Name: readCard.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月14日 星期二 15时21分20秒
 ************************************************************************/

#include "readCard.h"

#define DEVCHAR "/dev/ttyUSB0"

char pass[8] = {0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0x00, 0x00};
char auto_pass[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00};
struct card mCard;

int main() {

	int msgId, nread, ret;
	struct msg_buf msgBuf;
	struct sensor* card;
	struct card* mCard;
	struct custom* data;
	char recv[128];

	card = (struct sensor*)(msgBuf.data);
	mCard = (struct card*)(card->buf);
	data = (struct custom*) &(mCard->buf);
	card->type = RFID;

	int fd = open_port(DEVCHAR);
	if (fd<0) {
		fprintf(stderr, "%s open error ... \n", "xxxx");
		return -1;
	}

	msgId = ipc_msgCreat("/root/ipc_msg.c", 'c');
	if (msgId < 0) {
	
		perror("creat msg id error ... \n");
		return -1;
	}

	while (1) {

		nread = read(fd, recv, 128);
		
		PDEB("nread %d ... \n", nread);
		if (nread<0) {
			//sleep(1);
			continue;
		}

#if DEBUG_P
		int i;
		for (i=0;i<nread;i++) {

			printf("0x%02x ", recv[i]);
		}

		printf("\n");
#endif

	rfid_getId(fd, mCard);

	PDEB("get id %d ... \n", mCard->id);

	/* 读唯一号 */
	/*
	   0
	16

	32
	48 ...
	*/
		if (rfid_readData(fd, 0x0, 5, auto_pass, data->num)<0)  {//5个字节
			perror("read card error .........\n");
			continue;
		}

		msgBuf.mtype = CMD_BLE;
		ret = msgsnd(msgId, &msgBuf, sizeof(msgBuf.data), 0);//IPC_NOWAIT
		if(ret==-1)
		{
			perror("send message err\n");
			//return -1;
		}

	}
	return 0;
}
