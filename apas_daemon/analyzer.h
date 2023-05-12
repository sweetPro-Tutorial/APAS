#ifndef _ANALYZER_H_
#define _ANALYZER_H_


#include "../inc/common.h"
#include "../inc/statistics_info.h"
#include "../inc/air_info.h"
#include "../inc/list.h"

void analyzer(int threshold);  // 정보 분석
void makeStatistics();  // 통계 작성
void makeAlarmFile(int threshold);  // 대기오염 알람목록 작성
void stationStatistics(LinkedList *list);  // 측정소별 일평균 대기오염 통계 작성
int findStation(StatisticsInfo *stations, int count, char *stationName);  // 목록 내 측정소 검색:
                                                                          // 성공시 index, 실패시 -1
void processSum(StatisticsInfo *station, AirInfo *airInfo);  // 미세먼지/초미세먼지 관련 누계 작성
void getAirGrade(AirGrade *grade10, AirGrade *grade25, AirInfo *airInfo);  // 먼지 등급
void processAvg(StatisticsInfo *station);  // 미세먼지/초미세먼지 관련 평균 작성
void saveStations(StatisticsInfo *stations, int count);  // 통계 저장

void saveAlarms(LinkedList *alarmList);  // 알람 현황 저장


#endif