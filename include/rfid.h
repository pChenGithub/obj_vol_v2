/*************************************************************************
	> File Name: rfid.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月10日 星期五 17时04分21秒
 ************************************************************************/

#ifndef _RFID_H_
#define _RFID_H_

#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define uint_16 unsigned short
#define BSWAP_16(x) \
	(uint_16)((((uint_16)(x) & 0x00ff) << 8) | \
			(((uint_16)(x) & 0xff00) >> 8) \
			)

struct custom { //自定义的卡片存储区

	char num[5];
};

struct card {
	unsigned short mCardType;
	unsigned int id;
	struct custom buf;  //根据数据需求自定义的存储区
};

int rfid_writeData(int fd, const unsigned short addr, const unsigned short len, unsigned char pass[8], char *buff);
int rfid_readData(int fd, unsigned short addr, const unsigned short len, unsigned char pass[8], char *buff);
int rfid_chpass(int fd, int sector, char pass_old[8], char pass_new[8]);
int rfid_getId(int fd, struct card *mCard);
int rfid_autoMode(int fd, int type);

static int creat_bbc(char *buf, int len);
static int bcc_check();

#endif
