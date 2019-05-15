/*************************************************************************
	> File Name: libstore.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月13日 星期一 16时14分28秒
 ************************************************************************/

#include "store.h"

#define STOREFILE "./tmpdata.txt"

int store_vol_data(struct data_vol* data) {

	int fd, nw;
	char buf[32];
	int n = vol_prot(buf, 32, data);

	if (fd = open(STOREFILE, O_RDWR|O_CREAT|O_EXCL|O_APPEND, 0666)<0) {

		if(errno==EEXIST) {
		
			fd = open(STOREFILE, O_RDWR|O_APPEND);
		}else {
			perror("open store file error. \n");
			return -1;
		}
	}

	nw = write(fd, buf, n);
	if (nw!=n) {
	
		perror("write store file error. \n");
		nw = write(fd, buf, n);
	}
	//write(fd, "\r\n", 2);

	close(fd);

	return 0;
}

int store_xxx_data(struct data_vol* data) {

	return 0;
}
int read_data(char* buf, int len) {

	return 0;
}


