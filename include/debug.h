/*************************************************************************
	> File Name: debug.h
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月10日 星期五 16时47分29秒
 ************************************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#if defined DEBUG
#define PDEB(format, ...) printf(format, ##__VA_ARGS__)
#else
#define PDEB(format, ...)
#endif

#endif
