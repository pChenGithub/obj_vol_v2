/*************************************************************************
	> File Name: ../include/readCard.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月14日 星期二 15时35分12秒
 ************************************************************************/

#ifndef _READCARD_H_
#define _READCARD_H_

#include <stdio.h>
#include "debug.h"
#include "rfid.h"
#include "debug.h"
#include "ipc.h"
#include "sensor.h"


struct cmd_reqVol {

	int id;
	char req; //0 close,!0 open
};

struct cmd_rspVol {

	int id;
	char rsp; //0 close,!0 open
};

#endif
