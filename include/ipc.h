/*************************************************************************
	> File Name: ipc_msg.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年03月22日 星期五 14时56分21秒
 ************************************************************************/

#ifndef _IPC_H_
#define _IPC_H_

#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define MSG_LEN 32
struct msg_buf
{
	long mtype;
	char data[MSG_LEN];
};

enum chanel_t {
	CMD_BLE = 10,
	DATA_BLE,
	CMD_MAIN,
	DATA_MAIN,
};

int ipc_msgCreat(char *file, char c);

#endif
