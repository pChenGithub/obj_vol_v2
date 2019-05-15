/*************************************************************************
	> File Name: vol.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年03月22日 星期五 12时40分56秒
 ************************************************************************/

#include     <stdio.h>
#include     <stdlib.h>
#include     <unistd.h> 
#include     <sys/types.h> 
#include     <sys/stat.h> 
#include     <fcntl.h> 
#include     <termios.h>  
#include     <errno.h>
#include     <string.h>
#include 	<signal.h>
#include    <pthread.h>
#include	<time.h>
#include	"ipc.h"
#include "uart.h"
#include "sensor.h"
#include "debug.h"
#include "voltage.h"

//#define DEBUG_P 1

static struct termios newtios,oldtios; /*termianal settings */
char buff[101];
static char *vol_dev = "/dev/ttymxc2";

/********************************************
 *
 *		电量计数据读取---串口方式
 *
 * ******************************************/

int read_data_from_voltageSensor(int fd, char *buf, int len) {
	int n, i;
	char RX[48];
	char *start;
	int check;

	if (len<24) {
		perror("buff lenght should be 24");
		return -1;
	}

	PDEB("flush rx buff ... \n");
#ifndef TESTDEMO   //这个不测试的时候放开
	tcflush(fd,TCIFLUSH); //将要读数据了，我希望这个数据是新的，刷新一次输入缓存
#endif
	do {
		//sleep(3);
		n = read(fd, RX, 24);
		PDEB("can not read data ... \n");
	}while (n != 24);


	PDEB("read again sleep 3 s ... \n");
	//sleep(1);
	n = read(fd, RX+24, 24);
	if (n < 24) {
		perror("read data again error ... \n");
		//tcflush(fd,TCIFLUSH); //出错了，重新开始，刷新一次输入缓存
		return -1;
	}

#if DEBUG_P
	for (i=0;i<48;i++)
		printf("0x%02x ", RX[i]);

	printf("\n\n");
#endif

#if DEBUG_P
	printf("check data ... \n");
#endif

	i = 0;
	while (1) {
		if (i == 25 || ( (RX[i]&0xf0) == 0xf0 || RX[i] == 0x55 || RX[i] == 0xaa) && RX[i+1] == 0x5a )
			break;
		i++;
	}

	if ( i==25 ) {
		//tcflush(fd,TCIFLUSH); //出错了，重新开始，刷新一次输入缓存
		return -1;
	}else
		start = RX+i;

	check = 0;
	for (i=2;i<23;i++)
		check += start[i];

	if ( (check & 0xff) == start[23]) {
#if DEBUG_P
		printf("data right, copy ... \n");
#endif
		memcpy(buf, start, 24);
	}else {
		//tcflush(fd,TCIFLUSH); //出错了，重新开始，刷新一次输入缓存
		return -1;
	}

#if DEBUG_P
	printf("recv vol data ... \n");
	for (i=0;i<24;i++)
		printf("0x%02x ", start[i]);

	printf("\n\n");
#endif
	//tcflush(fd,TCIFLUSH); //每次读完数据之后刷新一次输入缓存

	return 0;
}

/********************************************
 *
 *		计算电压、电流、功率
 *
 * ******************************************/
#define V_E 1.88
#define I_E 1

int get_vol(char *in, int len_in, struct data_vol* data) {
	int vpr,ipr,ppr;
	int vr,ir,pr;
	//time_t seconds;

	vpr = in[2]*65536+in[3]*256+in[4];
	vr  = in[5]*65536+in[6]*256+in[7];
	ipr = in[8]*65536+in[9]*256+in[10];
	ir  = in[11]*65536+in[12]*256+in[13];
	ppr = in[14]*65536+in[15]*256+in[16];
	pr  = in[17]*65536+in[18]*256+in[19];

	data->v = 1.0*V_E*vpr/vr;
	data->i = 1.0*I_E*ipr/ir;
	data->p = 1.0*I_E*V_E*ppr/pr;

	PDEB("data: v %f i %f p %f ... \n", data->v, data->i, data->p);

#if 0
#if DEBUG_P
	printf("data 1: v %lf i %lf p %lf ... \n", *(double*)(out+2), *(double*)(out+12), *(double*)(out+22));

	//seconds = time(NULL);

	//printf("short size %d ... \n", sizeof(short) );
#endif
#endif
	return 0;
}

struct msg_buf msgBuf={

	.mtype = DATA_MAIN,
};


int main(int argc, char *argv[]) {

	int fd, msgId, ret;
	char *state = buff+20;
	struct data_vol *data;
	struct sensor *vol;

	vol = (struct sensor*)(msgBuf.data);
	data = (struct data_vol*)(vol->buf);

	vol->type = VOLTAGE;

	fd = open_port_mode1(vol_dev);
	if (fd<0) {
		fprintf(stderr, "%s open error ... \n", vol_dev);
		return -1;
	}

	/* ipc msg */
	msgId = ipc_msgCreat("/root/ipc_msg.c", 'c');
	if (msgId < 0) {     
		perror("creat msg id error ... \n");
		return -1;
	}

#ifdef TESTDEMO
	int i=0;
	while (i<500) {
		i++;
#else
	while (1) {
#endif

		PDEB("read data 24 byte ... \n");
#if 0
		while (read_data_from_voltageSensor(fd, buff, 24) != 0 ) //原始数据获取
			perror("read vol data error, again ... \n");
#else
		// 0x55 0x5a 0x02 0xec 0x70 0x00 0x05 0xbb 0x00 0x3f 0x3e 0x01 0x13 0xf8 0x51 0xa2 0x70 0x04 0x39 0xb1 0x71 0x00 0x05 0x6e
		buff[0] = 0x55;
		buff[1] = 0x5a;
		buff[2] = 0x02;
		buff[3] = 0xec;
		buff[4] = 0x70;
		buff[5] = 0x00;
		buff[6] = 0x05;
		buff[7] = 0xbb;
		buff[8] = 0x00;
		buff[9] = 0x3f;
		buff[10] = 0x3e;
		buff[11] = 0x01;
		buff[12] = 0x13;
		buff[13] = 0xf8;
		buff[14] = 0x51;
		buff[15] = 0xa2;
		buff[16] = 0x70;
		buff[17] = 0x04;
		buff[18] = 0x39;
		buff[19] = 0xb1;
		buff[20] = 0x71;
		buff[21] = 0x00;
		buff[22] = 0x05;
		buff[23] = 0x6e;
#endif
#if 0   //这个不测试的时候放开
		if ( !(*state&0x70) ){	/* 如果数据没有更新，continue */
			sleep(20);
			continue;
		}
#endif

		get_vol(buff, 24, data); //计算 v i p

		ret = msgsnd(msgId,&msgBuf, sizeof(msgBuf.data), 0);
		if(ret==-1)
		{
			perror("send message err\n");
		}

		sleep(FERQ); //不测试时改成16秒
	}

	close(fd);
	return 0;
}
