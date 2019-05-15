/*************************************************************************
	> File Name: mainApp.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月13日 星期一 14时43分23秒
 ************************************************************************/
#include "ipc.h"
#include "voltage.h"
#include "sensor.h"
#include "store.h"
#include <pthread.h>

struct arg {
	int fd;
	int *msgId;
};

static void* cmd_chanel(void *arg) {

	int ret;
	struct arg* parg = (struct arg*)arg;
	struct msg_buf msgBuf;
	int msgId = parg->msgId[0];

	while(1) {

		ret = msgrcv(msgId, &msgBuf, sizeof(msgBuf.data), CMD_MAIN, 0);
		if (ret==-1) {
			perror("recv message err \n");
			continue;
		}

		struct sensor *sensor = (struct sensor*)(msgBuf.data);
		switch (sensor->type) {
			case RFID:
				store_vol_data((struct data_vol*)(sensor->buf)); //拼接协议
				//给蓝牙让其发送
				//transfer_byBle

				break;
			default:
				break;
		}
	}
}


int main(int argc, char* argv[]) {
	
	struct msg_buf msgBuf;
	struct data_vol *vol;
	int msgId[2], ret;
	pthread_t ptid[5];

	/* ipc msg */
	msgId[0] = ipc_msgCreat("/root/ipc_msg.c", 'c');
	if (msgId < 0) {     
		perror("creat msg id error ... \n");
		return -1;
	}

	struct arg pArg;
	pArg.msgId = msgId;
	if (pthread_create (&ptid[0], NULL, cmd_chanel, (void*)&pArg) < 0) { //这个线程是main的指令通道
		perror("pthread uart_bleRead create error ... \n");
		return -1;
	}
	//线程分离

	while (1) { //这个线程是传感器至main的数据通道
	
		ret = msgrcv(msgId[0], &msgBuf, sizeof(msgBuf.data), DATA_MAIN, 0);
		if (ret==-1) {
			perror("recv message err \n");
			continue;
		}

		struct sensor *sensor = (struct sensor*)(msgBuf.data);

		switch (sensor->type) {
			case VOLTAGE:
				store_vol_data((struct data_vol*)(sensor->buf));
				break;
			default:
				break;
		}


	}

	return 0;
}


