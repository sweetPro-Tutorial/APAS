#ifndef _CONFIG_H_
#define _CONFIG_H_


#include "../inc/common.h"

typedef struct {
    String api_key;   // 인증키
    int    interval;  // 수집 주기(단위: 초)
    int    threshold; // 임계값
} Config;

// 일치하는 키의 값을 설정정보에 저장 : 성공 시 0, 실패 시 -1 반환
int keyHandler(void *config, String name, String value);

void printConfig(Config *config);


#endif