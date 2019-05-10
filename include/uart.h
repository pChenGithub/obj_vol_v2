/*************************************************************************
	> File Name: uart.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月10日 星期五 17时32分53秒
 ************************************************************************/

#ifndef _UART_H_
#define _UART_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>


int open_port(const char *portname);

#endif
