/****************************************************************************
 *                                                                          *
 * Copyright (c) 2014 Nuvoton Technology Corp. All rights reserved.         *
 *                                                                          *
 ****************************************************************************/
 
/****************************************************************************
 * 
 * FILENAME
 *     uart_test.c
 *
 * VERSION
 *     1.0
 *
 * DESCRIPTION
 *     This is the test program used to test the UARTs on NUC970 EV board
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *     None
 *
 * HISTORY
 *     
 *
 * REMARK
 *     None
 ****************************************************************************/
#include "rfid.h"

static char cmd[128] = {0xAA, 0xAB, 0x02, 0x10, 0x12};

/*****************************************
 *	写卡
 *	cmd 0x11
 *	len 写数据的长度
 *	AA AB 10 12 00 10 00 04 FF FF FF FF FF FF 61 62 63 64 12
 * **************************************/
int rfid_writeData(int fd, const unsigned short addr, const unsigned short len, unsigned char pass[8], char *buff) {

	int nread,i;
	char recv[6];

	cmd[2] = 14+len;
	cmd[3] = 0x12;
	*(unsigned short*)(cmd+4) = BSWAP_16(addr);
	*(unsigned short*)(cmd+6) = BSWAP_16(len);
	memcpy(cmd+8, pass, 8);
	memcpy(cmd+16, buff, len);

	if (creat_bbc(cmd, 17+len) < 0) {
	
		perror("bbc check creat error ... \n");
		return -1;
	}
#if DEBUG_P
	printf("cmd write \n");

	for (i=0;i<(17+len);i++)
		printf("0x%02x ", cmd[i]);

	printf("\n");
#endif
	write(fd, cmd, 17+len);

	sleep(1);

	nread = read(fd, recv, 6);
	if(nread!=6) {
	
		perror("write card error ... \n");
		return -1;
	}


	return 0;
}

/*****************************************
 *	读卡
 *	读写卡 一次只能在一个块里面读，不能跨块，
 *	在这个前提下地址、长度可以随意
 *	AA AB 0C 11 00 00 00 04 FF FF FF FF FF FF 00 00 19
 *	AA AB 07 91 00 31 31 31 31 96
 * **************************************/
int rfid_readData(int fd, unsigned short addr, const unsigned short len, unsigned char pass[8], char *buff) {
	
	int nread,i;
	char recv[128];
	int recv_len = 6+len;

	if (6+len >128) {
	
		perror("buff size not enough ... \n");
		return -1;
	}

	cmd[2] = 14;
	cmd[3] = 0x11;
	//*(unsigned short*)(cmd+4) = addr; //这边可能是大小端出现不能直接赋值的问题
	*(unsigned short*)(cmd+4) = BSWAP_16(addr);
	*(unsigned short*)(cmd+6) = BSWAP_16(len);
	memcpy(cmd+8, pass, 8);

	if (creat_bbc(cmd, 17) < 0) {
	
		perror("bbc check creat error ... \n");
		return -1;
	}

#if DEBUG_P
	printf("cmd read \n");

	for (i=0;i<17;i++)
		printf("0x%02x ", cmd[i]);

	printf("\n");
#endif
	write(fd, cmd, 17);

	//sleep(1);
	usleep(100000);

	nread = read(fd, recv, recv_len);
	if(nread!=recv_len ) {
	
		perror("read card error ... \n");
		return -1;
	}

	bcc_check();//检查收到的数据

	memcpy(buff, recv+5, len);

	return 0;
}

/*****************************************
 *	修改密码
 *	貌似这个指令的地址和长度没什么用(可以确定修改哪个扇区的密码)，
 *	每个扇区的密码是分开的
 *	AA AB 12 13 00 00 00 04 31 31 31 31 31 31 00 00 FF FF FF FF FF FF 00 00 05
 *	AA AB 03 93 00 90
 *
 * **************************************/
int rfid_chpass(int fd, int sector, char pass_old[8], char pass_new[8] ) {

	char recv[6];
	int nread,i;

	if (fd<0 || !pass_old || !pass_new) {
	
		perror("plese check param ... \n");
		return -1;
	}

	cmd[2] = 0x16;
	cmd[3] = 0x13;

	cmd[4] = 0x00;
	cmd[5] = 0x00;

	if (sector>0)
		cmd[5] = sector*32+16*(sector-1);
	
	cmd[6] = 0x00;
	cmd[7] = 0x04;

	memcpy(cmd+8, pass_new, 8);
	memcpy(cmd+16, pass_old, 8);

	if (creat_bbc(cmd, 25) < 0) {
	
		perror("bbc check creat error ... \n");
		return -1;
	}

#if DEBUG_P
	printf("cmd chpass \n");

	for (i=0;i<25;i++)
		printf("0x%02x ", cmd[i]);

	printf("\n");
#endif
	write(fd, cmd, 25);

	sleep(1);

	nread = read(fd, recv, 6);
	if(nread!=6 ) {
	
		perror("recv msg from card error ... \n");
		return -1;
	}

	if (recv[4]!=0x00) {
	
		perror("chpass recv msg staut not 0x00 ... \n");
		return -1;
	}

	return 0;
}

static int bcc_check() {

	return 0;
}

/*****************************************
 *	bbc创建
 *	buf 是可以保存整个命令的数组
 *	len 是整个命令的长度
 *
 * **************************************/
static int creat_bbc(char *buf, int len) {

	char *p = buf+2;
	int i;

	buf[len-1] = 0;

	for (i=0;i<len-3;i++)
		buf[len-1] ^= p[i];

	return 0;
} 

/*****************************************
 *	获取卡id，获取id的时候可以知道
 *	卡的类型,
 *	cmd 0x10,
 *	send AA AB 02 10 12
 *	recv AA AB 09 90 00 04 00 39 C8 8F 01 E2
 *		 AA AB 03 90 02 91  error
 *
 * **************************************/
int rfid_getId(int fd, struct card *mCard) {

	char buf[12]={0};
	int n;

	cmd[2] = 0x02;
	cmd[3] = 0x10;
	cmd[4] = 0x12;
#if DEBUG_P
	printf("get card id ... \n");
#endif
	if(fd<0) {
	
		perror("get id error ... \n");
		return -1;
	}

	if (write(fd, cmd, 5) < 0) {
		perror("send cmd get card id error ... \n");
		return -1;
	}
	
	sleep(1);
	n = read(fd, buf, sizeof(buf));
#if DEBUG_P
	printf("n read %d ... \n", n);
#endif
	if (n<0) {
		perror("recv msg error ... \n");
		return -1;
	}

	switch (n) {
	
		case  6:
			perror("can not read card id, check card ... \n");
			break;
		case 12:
			mCard->id = *(unsigned int*)(buf+7);
			mCard->mCardType = *(unsigned short*)(buf+5);
			break;
		defaut :
			perror("recv msg can not paess ... \n");
			break;
	}

	return 0;
}

/*****************************************
 	设置自动功能
	AA AB 03 2D 01 2F / AA AB 03 2D 00 2E / AA AB 03 2D 02 2C
	0：读卡号(默认)
	1：读数据
	2：空闲
 *
 * **************************************/
int rfid_autoMode(int fd, int type) {

	char buf[6];
	int n,i;

	cmd[2] = 0x03;
	cmd[3] = 0x2d;
#if DEBUG_P
	printf("set card mode ... \n");
#endif
	switch (type) {
	
		case 0:
			cmd[4] = 0x00;
			cmd[5] = 0x2e;
			break;
		case 1:
			cmd[4] = 0x01;
			cmd[5] = 0x2f;
			break;
		case 2:
			cmd[4] = 0x02;
			cmd[5] = 0x2c;
			break;
		defaut:
			perror("param invalid ... \n");
			return -1;
	}

	if (write(fd, cmd, 6) < 0) {
		perror("send cmd set autoMode error ... \n");
		return -1;
	}
	
	sleep(1);
	n = read(fd, buf, sizeof(buf));
#if DEBUG_P
	printf("n read %d ... \n", n);
#endif
	if (n!=6) {
		perror("recv msg error ... \n");
		return -1;
	}
#if DEBUG_P
	for (i=0;i<6;i++)
		printf("0x%02x ", buf[i]);

	printf("\n");
#endif
	if (buf[4]!=0x00) {
	
		perror("recv msg staut not 0x00 ... \n");
		return -1;
	}

	return 0;
}
