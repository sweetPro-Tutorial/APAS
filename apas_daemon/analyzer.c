#include "analyzer.h"
#include "collector.h"
// #include "../inc/list.h"
// #include "../inc/air_info.h"

void analyzer(int threshold) {
    makeStatistics();  // 측정소별 일평균 대기오염 통계 작성
    makeAlarmFile(threshold);  // 대기오염 알람목록 작성
}

void makeStatistics() {
    printWithTime(" >>> 측정소별 일평균 대기오염 통계 작성");

    // 당일 마스터 파일 로드
    char *data = loadFile(openMasterFile("rb", getTimeString(YYYYMMDD_HH)));
    if(data == NULL) { return; }

    // 대기오염정보 list 작성
    LinkedList list = { NULL, NULL, 0 };
    char *delimiter = "\n\r";
    char *token;
    char *savePointer;
    token = strtok_r(data, delimiter, &savePointer);
    while(token != NULL) {  // 마스터 데이터 순회
        // printf(">>> token : %s\n", token);  // test log
        // if(strlen(token)) {
        //     pushFront(&list, token);  // 대기오염정보 list 작성
        // }
        pushFront(&list, token);

        token = strtok_r(NULL, delimiter, &savePointer);
    }

    // list 를 순회하면서 
    //     측정소별로 일평균 농도와 일평균 등급 등 통계를 계산
    // 계산결과를 일자별 통계 파일에 저장
    stationStatistics(&list);

    free(data);
}

void stationStatistics(LinkedList *list) {  // 당일 통계 작성
    StatisticsInfo stations[MAX_STATIONS] = { 0, };

    int count = 0;
    AirInfo airInfo = { 0, };
    while(!isEmpty(list)) {
        char *csvString = getBeginNode(list)->data;
        csvToAirInfo(&airInfo, csvString);
        int index = findStation(stations, count, airInfo.stationName);
        if(index == FAIL) {  // 목록에 없음
            index = count;  // 빈 칸 할당
            count++;
        }
        processSum(&stations[index], &airInfo);  // 미세먼지/초미세먼지 관련 합계 작성
        processAvg(&stations[index]);  // 미세먼지/초미세먼지 관련 평균 작성
        popFront(list);
    }
    saveStations(stations, count);  // 통계 저장
}

int findStation(StatisticsInfo *stations, int count, char *stationName) {
    if(count == 0) { return FAIL; }
    for(int i = 0; i < count; i++) {
        if(match(stations[i].stationName, stationName)) { return i; }
    }    
    return FAIL;
}

void processSum(StatisticsInfo *station, AirInfo *airInfo) {
    AirGrade grade10;
    AirGrade grade25;
    getAirGrade(&grade10, &grade25, airInfo);
    if(grade10 != NO_DATA) {
        station->pm10_count++;
        station->pm10Grade1h_sum += atoi(airInfo->pm10Grade1h);
        station->pm10Value_sum += atoi(airInfo->pm10Value);
    }
    if(grade25 != NO_DATA) {
        station->pm25_count++;
        station->pm25Grade1h_sum += atoi(airInfo->pm25Grade1h);
        station->pm25Value_sum += atoi(airInfo->pm25Value);
    }

    station->sidoCode = airInfo->sidoCode;
    strcpy(station->sidoName, airInfo->sidoName);
    strcpy(station->stationName, airInfo->stationName);
}

void getAirGrade(AirGrade *grade10, AirGrade *grade25, AirInfo *airInfo) {
    if(match_n(airInfo->pm10Grade1h, "null", 4)) { 
        *grade10 = NO_DATA; 
    } else { 
        *grade10 = atoi(airInfo->pm10Grade1h); 
    }

    if(match_n(airInfo->pm25Grade1h, "null", 4)) { 
        *grade25 = NO_DATA; 
    } else { 
        *grade25 = atoi(airInfo->pm25Grade1h); 
    }
}

void processAvg(StatisticsInfo *station) {
    if(station->pm10_count > 0) {  // 0 으로 나눌 수는 없음
        station->pm10Value_average = ((double)station->pm10Value_sum / station->pm10_count);
        station->pm10Grade1h_average = ((double)station->pm10Grade1h_sum / station->pm10_count);
    }
    if(station->pm25_count > 0) { 
        station->pm25Value_average = ((double)station->pm25Value_sum / station->pm25_count);
        station->pm25Grade1h_average = ((double)station->pm25Grade1h_sum / station->pm25_count);
    }
}

void saveStations(StatisticsInfo *stations, int count) {
    if(count < 1) { return; }

    FILE *statisticsFile = openStatisticsFile("wb", getTimeString(YYYYMMDD));
    if(statisticsFile == NULL) { return; }

    StringLong csvString = { 0, };
    for(int i = 0; i < count; i++) {
        statisticsToCsv(csvString, &stations[i]);
        fprintf(statisticsFile, "\n%s", csvString);
    }
    fclose(statisticsFile);
}

void makeAlarmFile(int threshold) {
    printWithTime(" >>> 대기오염 알람목록 작성");

    // 응답 메시지에서 대기오염정보 list 추출
    LinkedList list = { NULL, NULL, 0 };
    if(!extractAirInfo(&list)) {
        eraseAllList(&list);
        return; 
    };

    // list 를 순회하면서 
    //     알람 발생 현황 작성
    LinkedList alarmList = { NULL, NULL, 0 };
    AirGrade grade10 = 0;
    AirGrade grade25 = 0;
    AirInfo airInfo = { 0, };
    while(!isEmpty(&list)) {  // 리스트 순회
        char *csvString = getBeginNode(&list)->data;
        csvToAirInfo(&airInfo, csvString);
        getAirGrade(&grade10, &grade25, &airInfo);
        if(grade10 >= threshold || grade25 >= threshold) {  // 알람목록 작성
            pushFront(&alarmList, csvString);
        }
        popFront(&list);
    }
    // 작성결과를 알람 현황 파일에 저장
    saveAlarms(&alarmList);
}

void saveAlarms(LinkedList *alarmList) {
    FILE *alarmFile = openAlarmFile("wb");  // 알람 파일 생성
    if(alarmFile == NULL) { return; }

    while(!isEmpty(alarmList)) {
        char *csvString = getBeginNode(alarmList)->data;
        fprintf(alarmFile, "\n%s", csvString);  // 알람 파일에 저장
        popFront(alarmList);
    }
    fclose(alarmFile);
}