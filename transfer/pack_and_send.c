/*************************************************************************
	> File Name: pack_and_send.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月14日 星期二 17时20分10秒
 ************************************************************************/
#include "readCard.h"
#include "ble.h"
#include "protocol.h"

static int ble_transfer(struct ble_transfer* tr, char head, char* buf, int len) {

	char npack, i;
	char send[10];
	//1byte 1byte 7byte
	//pack n
	if (len<=0) {
	
		perror("len > 0. \n");
		return -1;
	}
	npack = (len-1)/7 +1;
	PDEB("pack count %d. \n", npack);

	send[0] = head;
	send[1] = npack;
	send[2] = 0;

	for (i=0;i<npack;i++) {
		send_data(tr, send, 10);
		send[2] ++;
	}

	return 0;
}

int rfid_cmd_to_ble(struct ble_transfer* tr, struct cmd_reqVol* req) {

	char buf[32];
	char head; //bit 15 0cmd,

	head = 0xff;

	int n = cmd_rfid_prot(buf, 32, req);

	ble_transfer(tr, head ,buf, n);

	return 0;
}
