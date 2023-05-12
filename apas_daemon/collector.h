#ifndef _COLLECTOR_H_
#define _COLLECTOR_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../inc/common.h"
#include "../inc/list.h"

#define API                 "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty"
#define JEONKOOK_UTF8       "%EC%A0%84%EA%B5%AD"
#define MY_KEY              "xEmewenPvMHFZmo30BokoiiZ52hkL2UFhD5jMXafmLz04us" \
                            "8lGLu6T5JcuBJfnMHNw4h2F2yBTigAMn%2FMiTmKQ%3D%3D"

#define RESPONSE_FILENAME   "../data/response.data"

bool collector(String api_key);  // 대기오염정보 수집
bool requestAirData(String api_key);  // 정보연계: 대기정보 수신
bool extractAirInfo(LinkedList *list);  // 응답 메시지에서 원하는 정보 추출
bool isAlreadyProcessed(String dataTime);  // 중복되는 정보인지 확인
void saveAirInfo(LinkedList *list, String dataTime);  // 대기오염정보를 마스터 파일에 저장

#endif
