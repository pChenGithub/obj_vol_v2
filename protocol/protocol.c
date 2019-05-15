/*************************************************************************
	> File Name: protocol.c
	> Author: chen.peng
	> Mail: 404285202@qq.com 
	> Created Time: 2019年05月13日 星期一 16时37分20秒
 ************************************************************************/

#include "protocol.h"

/* 拼接电量计的最终传输数据格式,buf长度自己控制 */
int vol_prot(char* buf, int len, struct data_vol* data) {

	int strl=0;
	time_t seconds;
	struct tm tm;

	time(&seconds);
	localtime_r((const time_t*)&seconds, &tm);

	sprintf(buf, "vol:v%.2f:i%.2f:p%.2f:t%02d%02d%02d-%02d%02d%02d:vol\n", 
			data->v, data->i, data->p, 
			tm.tm_year, tm.tm_mon+1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec);

	PDEB("vol_prot: %s", buf);

	strl = strlen(buf);

	return strl;
}

//指令不需要时间戳,buf长度自己控制，不做判断
int cmd_rfid_prot(char* buf, int len, struct cmd_reqVol* req) {
	int strl=0;


	sprintf(buf, "rfid:req:id%d:v%d:rfid\n", 
			req->id, req->req); 

	PDEB("vol_prot: %s", buf);

	strl = strlen(buf);

	return strl;
}
