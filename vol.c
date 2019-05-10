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
#include "ipc_msg.h"


//#define DEBUG_P 1

static struct termios newtios,oldtios; /*termianal settings */
char buff[101];
static char *vol_dev = "/dev/ttymxc2";

int init_config() {

	return 0;
}

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

#if DEBUG_P
	printf("flush rx buff ... \n");
#endif
#ifndef TESTDEMO   //这个不测试的时候放开
	tcflush(fd,TCIFLUSH); //将要读数据了，我希望这个数据是新的，刷新一次输入缓存
#endif
	do {
		//sleep(3);
		n = read(fd, RX, 24);
#if DEBUG_P
		printf("can not read data ... \n");
#endif
	}while (n != 24);


#if DEBUG_P
	printf("read again sleep 3 s ... \n");
#endif
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

int get_vol(char *in, int len_in, char *out, int len_out) {
	int vpr,ipr,ppr;
	int vr,ir,pr;
	double v,i,p;
	//time_t seconds;

	vpr = in[2]*65536+in[3]*256+in[4];
	vr  = in[5]*65536+in[6]*256+in[7];
	ipr = in[8]*65536+in[9]*256+in[10];
	ir  = in[11]*65536+in[12]*256+in[13];
	ppr = in[14]*65536+in[15]*256+in[16];
	pr  = in[17]*65536+in[18]*256+in[19];

	v = 1.0*V_E*vpr/vr;
	i = 1.0*I_E*ipr/ir;
	p = 1.0*I_E*V_E*ppr/pr;

#if DEBUG_P
	printf("data: v %lf i %lf p %lf ... \n", v, i, p);
	//printf("double size %d ... \n", sizeof(double) );
#endif

	*(double*)(out+2) = v;
	*(double*)(out+12) = i;
	*(double*)(out+22) = p;


#if 0
#if DEBUG_P
	printf("data 1: v %lf i %lf p %lf ... \n", *(double*)(out+2), *(double*)(out+12), *(double*)(out+22));

	//seconds = time(NULL);

	//printf("short size %d ... \n", sizeof(short) );
#endif
#endif
	*out = 0;
	//*(char*)(out+1) = 1;
	*(out+10) = 1;
	//*(char*)(out+11) = 2;
	*(out+20) = 2;
	//*(char*)(out+21) = 4;

	return 0;
}

/********************************************
 *
 *		电量计串口打开模式
 *		波特率 4800 8数据位 1停止位 偶校验
 *
 * ******************************************/

static int open_port_mode1(const char *portname)
{
	int portfd;

#if DEBUG_P
	printf("opening serial port:%s\n",portname);
#endif
	/*open serial port */
	if((portfd=open(portname,O_RDWR | O_NOCTTY )) < 0 )
	{
   		printf("open serial port %s fail \n ",portname);
   		return portfd;
	}

	/*get serial port parnms,save away */
	tcgetattr(portfd,&newtios);
	memcpy(&oldtios,&newtios,sizeof newtios);
	/* configure new values */
	cfmakeraw(&newtios); /*see man page */
	newtios.c_iflag |=IGNPAR; /*ignore parity on input */
	newtios.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL); 
	
	//newtios.c_cflag = CS8 | CLOCAL | CREAD;
	newtios.c_cflag = CS8 | CLOCAL | CREAD | PARENB | INPCK;
        newtios.c_cflag &= ~PARODD;

	newtios.c_cc[VMIN]=1; /* block until 1 char received */
	newtios.c_cc[VTIME]=0; /*no inter-character timer */
	/* 115200 bps */
	cfsetospeed(&newtios,B4800);
	cfsetispeed(&newtios,B4800);
	/*apply modified termios */
	tcflush(portfd,TCIFLUSH);
	tcsetattr(portfd,TCSADRAIN,&newtios);
	return portfd;
}

int main(int argc, char *argv[]) {

	int fd, msgId, ret;
	char TX[30] = {0};
	char *state = buff+20;
	struct msg_buf msgBuf;

	init_config();

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


	msgBuf.mtype = 300;

#ifdef TESTDEMO
	int i=0;
	while (i<500) {
		i++;
#else
	while (1) {
#endif

#if DEBUG_P
		printf("read data 24 byte ... \n");
#endif
		while (read_data_from_voltageSensor(fd, buff, 24) != 0 )
			perror("read vol data error, again ... \n");

#ifndef TESTDEMO   //这个不测试的时候放开
		if ( !(*state&0x70) ){	/* 如果数据没有更新，continue */
			sleep(20);
			continue;
		}
#endif

		get_vol(buff, 24, TX, 30);

		memcpy(msgBuf.data, TX, 10);

		ret = msgsnd(msgId,&msgBuf, sizeof(msgBuf.data), 0);
		if(ret==-1)
		{
			printf("send message err\n");
			perror("senderr");
		}
		//sleep(1);
		usleep(100000);


		memcpy(msgBuf.data, TX+10, 10);

		ret = msgsnd(msgId,&msgBuf, sizeof(msgBuf.data), 0);
		if(ret==-1)
		{
			printf("send message err\n");
			perror("senderr");
		}
		//sleep(1);
		usleep(100000);


		memcpy(msgBuf.data, TX+20, 10);

		ret = msgsnd(msgId,&msgBuf, sizeof(msgBuf.data), 0);
		if(ret==-1)
		{
			printf("send message err\n");
			perror("senderr");
		}
#ifndef TESTDEMO
		printf("vol release demo ... \n");
		sleep(20); //不测试时改成16秒
#else
		printf("vol test demo ... \n");
		sleep(2); //test
#endif
	}

	close(fd);
	return 0;
}
