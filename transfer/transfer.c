/*************************************************************************
	> File Name: transfer.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月14日 星期二 10时27分09秒
 ************************************************************************/

#include "transfer.h"

#define DEVCHAR "/dev/ttymxc1"

struct ble_transfer transfer={
	0,
	{0x4f, 0x45, 0x6f, 0x65, 0x00, 0x80, 0x00},
	{0},
	{0, 0,},
};

static void* data_chanel(void *arg) {

	int ret;
	struct msg_buf msgBuf;
	struct arg* parg = (struct arg*)arg;
	int msgId = parg->msgId;
	while(1) {
	}
}


static void* cmd_chanel(void *arg) {

	int ret;
	struct arg* parg = (struct arg*)arg;
	struct msg_buf msgBuf;
	int msgId = parg->msgId;
	int fd = parg->fd;

	while(1) { //接收到蓝牙的指令

		ret = msgrcv(msgId, &msgBuf, sizeof(msgBuf.data), CMD_BLE, 0);
		if (ret==-1) {
			perror("recv message err \n");
			continue;
		}

		struct sensor *sensor = (struct sensor*)(msgBuf.data);
		struct cmd_reqVol* req;
		switch (sensor->type) {
			case RFID:
				//参考协议打包发送
				rfid_cmd_to_ble(&transfer, req);
				break;
			default:
				break;
		}
	}
}

int main(int argc, char* argv[]) {

	int fd, msgId;
	pthread_t ptid[2];
	struct arg pArg;
	char buf[10];


	fd = open_port(DEVCHAR);
	if (fd<0) {
		fprintf(stderr, "%s open error ... \n", DEVCHAR);
		return -1;
	}

	msgId = ipc_msgCreat("/root/ipc_msg.c", 'c');
	if (msgId < 0) {
		perror("creat msg id error ... \n");
		return -1;
	}

	pArg.msgId = msgId;
	pArg.fd = fd;

	if (pthread_create (&ptid[0], NULL, cmd_chanel, (void*)&pArg) < 0) { //这个线程是传感器至main的指令通道
		perror("pthread uart_bleRead create error ... \n");
		return -1;
	}
	//线程分离

	while(1) {
	
		if (recv_data(&transfer, buf, 10) < 0) {
			perror("recv data from ble error ... \n");
			continue;
		}

		switch (buf[0]) {
		
			case 0xff://是指令
				break;
			default://是数据

				break;
		}

	}
	return 0;
}
