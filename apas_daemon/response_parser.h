#ifndef _RESPONSE_PARSER_H_
#define _RESPONSE_PARSER_H_


#include "../inc/list.h"
#include "../inc/common.h"
#include "../inc/air_info.h"

void responseParse(LinkedList *list, char *data);  // 응답 메시지 파싱
bool parseAirData(AirInfo *airInfo, char *airData);  // 측정소별 측정정보 파싱


#endif
