#include "statistics_info.h"

char *statisticsToCsv(StringLong csvString, StatisticsInfo *station) {
    memset(csvString, 0x00, LINE_SIZE);
    sprintf(csvString, "%d,%s,%s,"
                "%f,%f,"
                "%d,%d,%d,"
                "%f,%f,"
                "%d,%d,%d",
            station->sidoCode, station->sidoName, station->stationName,
            station->pm10Value_average, station->pm10Grade1h_average,
            station->pm10Value_sum, station->pm10Grade1h_sum,station->pm10_count,
            station->pm25Value_average, station->pm25Grade1h_average,
            station->pm25Value_sum, station->pm25Grade1h_sum,station->pm25_count);


    return csvString;
}

StatisticsInfo *csvToStatisticsInfo(StatisticsInfo *statisticsInfo, StringLong csvString) {
    char *delimiter = ",";
    char *token;
    char *savePoint;
    StringLong tempString = { 0, };
    
    memset(statisticsInfo, 0x00, sizeof(StatisticsInfo));
    strncpy(tempString, csvString, LINE_SIZE - 1);
    token = strtok_r(tempString, delimiter, &savePoint);
    int i = 0;
    while(token != NULL) {
        switch(i) {
        case  0: statisticsInfo->sidoCode = atoi(token); break;
        case  1: strcpy(statisticsInfo->sidoName, token); break;
        case  2: strcpy(statisticsInfo->stationName, token); break;
        case  3: statisticsInfo->pm10Value_average = atof(token); break;
        case  4: statisticsInfo->pm10Grade1h_average = atof(token); break;
        case  5: statisticsInfo->pm10Value_sum = atoi(token); break;
        case  6: statisticsInfo->pm10Grade1h_sum = atoi(token);break;
        case  7: statisticsInfo->pm10_count = atoi(token);break;
        case  8: statisticsInfo->pm25Value_average = atof(token); break;
        case  9: statisticsInfo->pm25Grade1h_average = atof(token); break;
        case 10: statisticsInfo->pm25Value_sum = atoi(token); break;
        case 11: statisticsInfo->pm25Grade1h_sum = atoi(token);break;
        case 12: statisticsInfo->pm25_count = atoi(token);break;
        default: break;
        }
        i++;
        token = strtok_r(NULL, delimiter, &savePoint);
    }

    return statisticsInfo;
}