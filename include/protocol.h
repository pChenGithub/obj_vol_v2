/*************************************************************************
	> File Name: ../include/protocol.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月13日 星期一 16时48分38秒
 ************************************************************************/

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "voltage.h"
#include <string.h>
#include <stdio.h>
#include "debug.h"
#include <time.h>
#include "readCard.h"

int vol_prot(char* buf, int len, struct data_vol* data);
int cmd_rfid_prot(char* buf, int len, struct cmd_reqVol* req);

#endif
