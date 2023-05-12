#ifndef _AIR_INFO_H_
#define _AIR_INFO_H_


#include "common.h"

typedef struct {
    String dataTime;
    int    sidoCode;
    String sidoName;
    String stationName;

    String pm10Value;
    String pm10Value24;
    String pm10Grade1h;
    String pm10Grade;
    String pm10Flag;

    String pm25Value;
    String pm25Value24;
    String pm25Grade1h;
    String pm25Grade;
    String pm25Flag;
} AirInfo;

typedef enum { NO_DATA, GOOD, NORMAL, BAD, VERY_BAD } AirGrade;

//
// helper function
//
bool isAirData(char *data);
char *airInfoToString(AirInfo *airInfo);  // AirInfo 정보를 문자열로 변환
char *airInfoToCsv(AirInfo *airInfo);  // AirInfo 정보를 .csv 포맷의 문자열로 변환
AirInfo *csvToAirInfo(AirInfo *airInfo, char *csvString);  // .csv 포맷의 문자열을 AirInfo로 변환


#endif