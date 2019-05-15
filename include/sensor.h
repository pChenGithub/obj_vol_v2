/*************************************************************************
	> File Name: sensor.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月10日 星期五 18时50分44秒
 ************************************************************************/

#ifndef _SENSOR_H_
#define _SENSOR_H_

enum sensor_t {

	RFID = 100,
	VOLTAGE,
};

struct sensor {

	enum sensor_t type;
	char buf[32];
};

#endif
