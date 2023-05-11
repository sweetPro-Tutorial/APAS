#ifndef _COLLECTOR_H_
#define _COLLECTOR_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define API                 "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty"
#define JEONKOOK_UTF8       "%EC%A0%84%EA%B5%AD"
#define MY_KEY              "xEmewenPvMHFZmo30BokoiiZ52hkL2UFhD5jMXafmLz04us" \
                            "8lGLu6T5JcuBJfnMHNw4h2F2yBTigAMn%2FMiTmKQ%3D%3D"

#define RESPONSE_FILENAME   "../data/response.data"
#define LINE_SIZE           1024  // 긴   문자열용
#define ITEM_SIZE           100   // 짧은 문자열용

typedef char StringLong[LINE_SIZE];
typedef char String[ITEM_SIZE];

bool collector();  // 대기오염정보 수집


#endif

