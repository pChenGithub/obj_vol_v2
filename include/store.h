/*************************************************************************
	> File Name: store.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月13日 星期一 16时16分37秒
 ************************************************************************/

#ifndef _STORE_H_
#define _STORE_H_

#include "voltage.h"
#include "protocol.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int store_vol_data(struct data_vol* data);

#endif
