#include "response_parser.h"
#include "../inc/air_info.h"

void responseParse(LinkedList *list, char *data) {
    // 검색할 키 목록을 준비;  --> 총 13개(sidoName ~ dataTime)
    // 응답 메시지를 한 객체씩 순회하면서
    printWithTime(" >>> 응답 메시지 파싱");

    char *delimiter = "{}[]\n\t";
    char *token;
    char *savePointer;
    token = strtok_r(data, delimiter, &savePointer);
    while(token != NULL) {
        // printf(">>> token : %s\n", token);  // test log
        if(!isAirData(token)) {   // 유효한 객체인지 확인
            token = strtok_r(NULL, delimiter, &savePointer);
            continue;
        }
        AirInfo airInfo;
        if(!parseAirData(&airInfo, token)) {  // 정보 추출
            token = strtok_r(NULL, delimiter, &savePointer);
            continue;
        };          
        pushFront(list, airInfoToCsv(&airInfo));  // 추출한 정보로 리스트 작성

        token = strtok_r(NULL, delimiter, &savePointer);
    }
}

bool parseAirData(AirInfo *airInfo, char *airData) {
    char *delimiter = ",:\"";
    char *token;
    char *savePoint;
    char tempData[LINE_SIZE] = { 0, };

    strncpy(tempData, airData, LINE_SIZE - 1);
    // printf(">>> parseAirData: %s\n", tempData);
    token = strtok_r(tempData, delimiter, &savePoint);
    memset(airInfo, 0x00, sizeof(AirInfo));
    while(token != NULL) {
        // printf(">>> parseAirData token : %s\n", token);  // test log
        if(match(token, "dataTime")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->dataTime, token);
        } else if(match(token, "sidoName")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->sidoName, token);
            
            int sidoCode = sidoNameToCode(airInfo->sidoName);
            if(sidoCode == FAIL) { 
                printf("ERROR|[%s:%d] sidoName(%s) not found.", 
                    __FILE__, __LINE__, airInfo->sidoName);  // error log
                return false; 
            } 
            airInfo->sidoCode = sidoCode;
        } else if(match(token, "stationName")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->stationName, token);
        
        } else if(match(token, "pm10Value")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm10Value, token);
        } else if(match(token, "pm10Value24")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm10Value24, token);
        } else if(match(token, "pm10Grade1h")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm10Grade1h, token);
        } else if(match(token, "pm10Grade")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm10Grade, token);
        } else if(match(token, "pm10Flag")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm10Flag, token);
        
        } else if(match(token, "pm25Value")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm25Value, token);
        } else if(match(token, "pm25Value24")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm25Value24, token);
        } else if(match(token, "pm25Grade1h")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm25Grade1h, token);
        } else if(match(token, "pm25Grade")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm25Grade, token);
        } else if(match(token, "pm25Flag")) { 
            token = strtok_r(NULL, delimiter, &savePoint);
            strcpy(airInfo->pm25Flag, token);
        }

        if(token != NULL) {  // 다음 토큰 추출
            token = strtok_r(NULL, delimiter, &savePoint); 
        };  
    }
    // printf("%s\n", airInfoToString(airInfo));
    return true;
}