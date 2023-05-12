#ifndef _CONFIG_INFO_H_
#define _CONFIG_INFO_H_


#include "../inc/common.h"

typedef struct {
    int  threshold;
} Config;

// 일치하는 키의 값을 설정정보에 저장
int keyHandler(void *config, char *name, char *value);

//
// helper function
//
void printConfig(Config *config);


#endif