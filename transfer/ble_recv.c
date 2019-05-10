/*************************************************************************
	> File Name: ble_recv.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年04月11日 星期四 14时18分13秒
 ************************************************************************/


#include "ble_recv.h"

static char TXbuf[17] = {0x4f, 0x45, 0x6f, 0x65, 0x00, 0x80, 0x00};
static char RXbuf[18] = {0};

struct ble_msgCount msgCount = {0};

#if defined JO_MODE_BLE
/********************************************
 *
 *		金瓯模块的发送方式 
 *		与JDY-10M模块通信方式（串口方面）一样
 *
 * ******************************************/

//static char *head="AT+SENDDATA=0,12345";

int send_data(int fd, char *buf, int len) {

	int n,i;
#if DEBUG_P
	printf("send data by jo ...\n");
#endif
	if (len>10) {
		perror("len too long ...\n");
		return -1;
	}

#if DEBUG_P
	for (i=0;i<10;i++)
		printf("0x%02x ", buf[i]);
	printf("\n\n");
#endif

	n = write(fd, buf, 10);
	if (n<0)
		perror("send data error ...\n");
#if DEBUG_P
	else
		printf("send %d byte ...\n", n);
#endif
	return n;

}

#else
/********************************************
 *
 *		偶亿模块的发送方式
 *
 * ******************************************/
int send_data(int fd, char *buf, int len) {
	int n,i;
	char *p;
	
#if DEBUG_P
	printf("send data by ole ...\n");
#endif

	if (len>10) {
		perror("len too long ...\n");
		return -1;
	}

	p = TXbuf+7;

	memset(p, 0, 10);
	memcpy(p, buf, len);

//#if DEBUG_P
#if 1
	printf("send data by ole ...\n");
	for (i=0;i<17;i++)
		printf("0x%02x ", TXbuf[i]);
	printf("\n\n");
#endif

	n = write(fd, TXbuf, 17);
	if (n<0)
		perror("send data error ...\n");
#if DEBUG_P
	else
		printf("send %d byte ...\n", n);
#endif
	
	return n;
}
#endif

/********************************************
 *
 *		蓝牙透传数据接收---还未实现
 *
 * ******************************************/
int recv_data(int fd, char *buf, int len) {

	int n, i;
	char *p = RXbuf+8;

	if (len<10) {
		perror("len too short ... \n");
		return -1;
	}
	memset(RXbuf, 0, 18);
	n = read(fd, RXbuf, 18);
	if (n<=0) {
		perror("read uart error ... \n");
		return -1;
	}
#if DEBUG_P	
	printf("read data from ble ... \n");

	for (i=0;i<18;i++)
		printf("0x%02x ", RXbuf[i]);
	printf("\n\n");
#endif	

	memcpy(buf, p, len);
	return n;
}

int ble_getMsgCount() {

	return 0;
}

int ble_init() {

	return 0;
}
