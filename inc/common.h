#ifndef _COMMON_H_
#define _COMMON_H_


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define LINE_SIZE           1024  // 긴   문자열용
#define ITEM_SIZE           100   // 짧은 문자열용

#define SUCCESS       0
#define FAIL          -1
#define END           '\0'
#define NEWLINE       '\n'
#define NUMBER        "1234567890"

#define SIDO_TABLE_SIZE  18
#define NATION            0

typedef char StringLong[LINE_SIZE];
typedef char String[ITEM_SIZE];

//
// sido table
//
int sidoNameToCode(String sidoName);  // 시도 이름을 코드로 변환:
                                      // 성공시 코드값, 실패시 -1 반환
char *sidoCodeToName(int sidoCode);   // 시도 코드를 이름으로 변환

//
// file
//
#define MAX(a, b)   ((a) > (b) ? (a) : (b))

FILE *openFile(char *filename, char *mode);
char *loadFile(FILE *fp);  // 파일을 동적 메모리로 로드 후 파일 닫음: 
                           // 성공시 메모리 주소, 실패시 NULL 반환
                           // 주의: 사용 후 반드시 free 필요
long fileSize(FILE *fp);
void getLastLine(char *line, FILE *fp);  // 텍스트 파일에서 마지막 줄 추출
FILE *openMasterFile(char *mode, String date);  // 날짜로 마스터 파일 오픈
FILE *openStatisticsFile(char *mode, String date);  // 날짜로 통계 파일 오픈
FILE *openAlarmFile(char *mode);  // 알람 파일 오픈

void printWithTime(char *message);
void printTitle(char *title);

//
// string handling
//
char *strStrip(char *string);  // 문자열 앞 뒤 공백 제거
char *strRStrip(char *string);  // 문자열 뒤 공백 제거
char *strLStrip(char *string);  // 문자열 앞 공백 제거
char *leftSkip(char *string);  // 문자열에서 공백이 아닌 첫 번째 문자의 위치 찾기


char *strLowerCase(char *string);  // 문자열을 소문자로 만듬
char *strUpperCase(char *string);  // 문자열을 대문자로 만듬
bool match(char *str1, char *str2);  // 문자열 일치 여부 반환
bool match_n(char *str1, char *str2, int count);  // 앞에서 n 문자열 일치 여부 반환


//
// time
//
typedef enum { YYYYMMDD, YYYYMMDD_HH, YYYYMMDD_HH_00, YYYYMMDD_HHmmSS } DateFormat;
char *getTimeString(DateFormat kind);  // 현재 시간을 문자열로 만듬: 
                                       // eg) 2023-01-31 21:00:59

void keyboardBufferCleansing();


#endif
