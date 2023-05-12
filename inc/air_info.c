#include "air_info.h"

static StringLong tempString = { 0, };

bool isAirData(char *data) {
    if(strstr(data, "pm25Grade1h") != NULL) { return true; }

    return false;
}

char *airInfoToString(AirInfo *airInfo) {
    sprintf(tempString, "AirInfo=["
            "dataTime=%s,sidoCode=%d,sidoName=%s,stationName=%s,"
            "pm10Value=%s,pm10Value24=%s,pm10Grade1h=%s,pm10Grade=%s,pm10Flag=%s,"
            "pm25Value=%s,pm25Value24=%s,pm25Grade1h=%s,pm25Grade=%s,pm25Flag=%s"
            "]\n", 
        airInfo->dataTime, airInfo->sidoCode, airInfo->sidoName, airInfo->stationName,
        airInfo->pm10Value, airInfo->pm10Value24, airInfo->pm10Grade1h, airInfo->pm10Grade, airInfo->pm10Flag,
        airInfo->pm25Value, airInfo->pm25Value24, airInfo->pm25Grade1h, airInfo->pm25Grade, airInfo->pm25Flag );

    return tempString;
}

char *airInfoToCsv(AirInfo *airInfo) {
    memset(tempString, 0x00, LINE_SIZE);
    sprintf(tempString, "%s,%d,%s,%s,"
                  "%s,%s,%s,%s,%s,"
                  "%s,%s,%s,%s,%s", 
        airInfo->dataTime, airInfo->sidoCode, airInfo->sidoName, airInfo->stationName,
        airInfo->pm10Value, airInfo->pm10Value24, airInfo->pm10Grade1h, airInfo->pm10Grade, airInfo->pm10Flag,
        airInfo->pm25Value, airInfo->pm25Value24, airInfo->pm25Grade1h, airInfo->pm25Grade, airInfo->pm25Flag );

    return tempString;
}

AirInfo *csvToAirInfo(AirInfo *airInfo, char *csvString) {
    char *delimiter = ",";
    char *token;
    char *savePoint;
    char tempString[LINE_SIZE] = { 0, };
    
    memset(airInfo, 0x00, sizeof(AirInfo));
    strncpy(tempString, csvString, LINE_SIZE - 1);
    token = strtok_r(tempString, delimiter, &savePoint);
    int i = 0;
    while(token != NULL) {
        switch(i) {
        case  0: strcpy(airInfo->dataTime, token); break;
        case  1: airInfo->sidoCode = atoi(token); break;
        case  2: strcpy(airInfo->sidoName, token); break;
        case  3: strcpy(airInfo->stationName, token); break;
        case  4: strcpy(airInfo->pm10Value, token); break;
        case  5: strcpy(airInfo->pm10Value24, token); break;
        case  6: strcpy(airInfo->pm10Grade1h, token); break;
        case  7: strcpy(airInfo->pm10Grade, token); break;
        case  8: strcpy(airInfo->pm10Flag, token); break;
        case  9: strcpy(airInfo->pm25Value, token); break;
        case 10: strcpy(airInfo->pm25Value24, token); break;
        case 11: strcpy(airInfo->pm25Grade1h, token); break;
        case 12: strcpy(airInfo->pm25Grade, token); break;
        case 13: strcpy(airInfo->pm25Flag, token); break;
        default: break;
        }
        i++;
        token = strtok_r(NULL, delimiter, &savePoint);
    }

    return airInfo;
}



