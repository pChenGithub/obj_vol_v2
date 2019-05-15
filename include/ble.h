/*************************************************************************
	> File Name: ble_recv.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年04月11日 星期四 14时19分00秒
 ************************************************************************/

#ifndef _BLE_H_
#define _BLE_H_

#include<stdio.h>
#include <string.h>
#include "debug.h"

struct ble_msgCount {

	int recv;
	int send;
};

struct ble_transfer {

	int fd;
	char TXbuf[17];
	char RXbuf[18];
	struct ble_msgCount msgCount;
};

int ble_getMsgCount();
int ble_init();
int recv_data(struct ble_transfer* tr, char *buf, int len);
int send_data(struct ble_transfer* tr, char *buf, int len);

#endif

