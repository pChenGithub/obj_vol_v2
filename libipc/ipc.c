/*************************************************************************
	> File Name: ipc_msg.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年03月22日 星期五 13时41分09秒
 ************************************************************************/

#include "ipc.h"

int ipc_msgCreat(char *file, char c) {

	key_t key;
	int msgId;

	//系统建立IPC通讯 （消息队列、信号量和共享内存） 时必须指定一个ID值。通常情况下，该id值通过ftok函数得
	key = ftok(file, c);
	if (key<0) {
	
		perror("get ipc msg key error ... \n");
		return -1;
	}

	printf("get ipc msg key %d ... \n", key);

	//用来创建和访问一个消息队列
	msgId = msgget(key, IPC_CREAT |IPC_EXCL|0666); //通过文件对应

	if (msgId==-1) {
		if (errno == EEXIST)
		{
			printf("EEXIST:.....\n");
			key=ftok(file, c);
			msgId=msgget(key, IPC_CREAT|0666); //通过文件对应
		}
		else
		{
			perror("msget error ... \n");
			return -1;
		}
	}

	printf("msgid:%d \n", msgId);

	return msgId;
}

