#ifndef _STATISTICS_INFO_H_
#define _STATISTICS_INFO_H_


#include "../inc/common.h"

#define MAX_STATIONS    1000

typedef struct {
    int    sidoCode;
    String sidoName;
    String stationName;
    double pm10Value_average;
    double pm10Grade1h_average;
    int    pm10Value_sum;
    int    pm10Grade1h_sum;
    int    pm10_count;
    double pm25Value_average;
    double pm25Grade1h_average;
    int    pm25Value_sum;
    int    pm25Grade1h_sum;
    int    pm25_count;
} StatisticsInfo;

char *statisticsToCsv(StringLong csvString, StatisticsInfo *station);  // 통계정보를 csv 포맷 문자열로 변환
StatisticsInfo *csvToStatisticsInfo(StatisticsInfo *statisticsInfo, StringLong csvString);  // csv 포맷 문자열을 통계정보로 변환


#endif