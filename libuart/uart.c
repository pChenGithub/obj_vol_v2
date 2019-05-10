/*************************************************************************
	> File Name: uart.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月10日 星期五 17时25分50秒
 ************************************************************************/

#include "uart.h"
#include "debug.h"

static struct termios newtios,oldtios; /*termianal settings */

int open_port(const char *portname)
{
	int portfd;

	PDEB("opening serial port:%s\n",portname);

	/*open serial port */
	if((portfd=open(portname,O_RDWR | O_NOCTTY )) < 0 )
	{
   		fprintf(stderr, "open serial port %s fail \n ",portname);
   		return portfd;
	}

	/*get serial port parnms,save away */
	tcgetattr(portfd,&newtios);
	memcpy(&oldtios,&newtios,sizeof(newtios));
	/* configure new values */
	cfmakeraw(&newtios); /*see man page */
	newtios.c_iflag |=IGNPAR; /*ignore parity on input */
	newtios.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL); 
	newtios.c_cflag = CS8 | CLOCAL | CREAD;
	newtios.c_cc[VMIN]=1; /* block until 1 char received */
	newtios.c_cc[VTIME]=0; /*no inter-character timer */
	/* 115200 bps */
	cfsetospeed(&newtios,B115200);
	cfsetispeed(&newtios,B115200);

	tcflush(portfd,TCIFLUSH);
	tcflush(portfd,TCOFLUSH);
	tcsetattr(portfd,TCSADRAIN,&newtios);
	return portfd;
}
