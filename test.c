/*************************************************************************
	> File Name: test.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月10日 星期五 16时51分37秒
 ************************************************************************/

#include<stdio.h>
#include "debug.h"
#include "rfid.h"
#include "uart.h"

#if 0
int main() {

	PDEB("adfafd\n");
	PDEB("adfafd %d \n", 13);
	return 0;
}
#endif

#if 0

char pass[8] = {0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0x00, 0x00};
char auto_pass[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00};
struct card mCard;


int main() {

	int fd = open_port("/dev/ttyACM0");
	if (fd<0) {
		fprintf(stderr, "%s open error ... \n", "xxxx");
		return -1;
	}

	rfid_getId(fd, &mCard);

	printf("get id %d ... \n", mCard.id);

	if (rfid_chpass(fd, 0, auto_pass, pass) < 0 ) {
		perror("chpass error ... \n");
	}

	if (rfid_chpass(fd, 0, pass, auto_pass) < 0 ) {
		perror("chpass error ... \n");
	}

	return 0;
}
#endif
