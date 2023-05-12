#include "common.h"

static char *sidoTable[SIDO_TABLE_SIZE] = {
    "전국", "서울", "부산", "대구", "인천", 
    "광주", "대전", "울산", "경기", "강원",
    "충북", "충남", "전북", "전남", "경북", 
    "경남", "제주", "세종"
};

// static StringLong tempString;


int sidoNameToCode(String sidoName) {
    for(int i = 0; i < SIDO_TABLE_SIZE; i++) {
        if(match(sidoName, sidoTable[i])) { return i; }
    }
    return FAIL;
}

char *sidoCodeToName(int sidoCode) {
    return sidoTable[sidoCode];
}


FILE *openFile(char *filename, char *mode) {
    FILE *fp = fopen(filename, mode);
    if(fp == NULL) {
        printf("ERROR|[%s:%s:%d] %s: %s\n", 
            __FILE__, __func__, __LINE__, filename, strerror(errno));
    }
    return fp;
}

char *loadFile(FILE *fp) {
    if(fp == NULL) { return NULL; }

    long bufferSize = fileSize(fp);
    if(bufferSize == FAIL) { return NULL; }

    char *data = calloc(bufferSize + 1, 1);
    if(data == NULL) { return NULL; }
    fread(data, 1, bufferSize, fp);
    fclose(fp);

    return(data);
}

long fileSize(FILE *fp) {
    if(fp == NULL) { return FAIL; }

    long currentOffset = ftell(fp);  // 현재 위치 저장
    fseek(fp, 0, SEEK_END);
    long offset = ftell(fp);
    fseek(fp, currentOffset, SEEK_SET);  // 기존 위치로 이동

    return offset;
}

void getLastLine(char *line, FILE *fp) {  // 텍스트 파일에서 마지막 줄 추출
    if(fp == NULL) { return; }

    // 파일의 뒤쪽으로 이동
    int size = 0;
    long fsize = -1L*fileSize(fp);
    if(fseek(fp, MAX(fsize, -4096L), SEEK_END) != SUCCESS) { return; }

    // 뒤에서부터 마지막 줄 찾기
    char buffer[4096 + 1] = { 0, };
    size = fread(buffer, 1, 4096, fp);
    buffer[size] = END;  // 문자열 끝 표시
    int i;
    for(i = size - 1; i >= 0; i--) {
        if(buffer[i] == NEWLINE) { break; }
    }
    i++;
    strcpy(line, &buffer[i]);
}

FILE *openMasterFile(char *mode, String date) {
    String filename = { 0, };
    sprintf(filename, "../data/%.10s.csv", date);
    return openFile(filename, mode);
}

FILE *openStatisticsFile(char *mode, String date) {
    String filename = { 0, };
    sprintf(filename, "../data/%.10s_stat.csv", date);
    return openFile(filename, mode);
}

FILE *openAlarmFile(char *mode) {
    String filename = { 0, };
    sprintf(filename, "../data/alarm.csv");
    return openFile(filename, mode);
}





void printWithTime(char *message) {
    String tempString;
    time_t temp = time(NULL);
    struct tm *timePointer;
    timePointer = localtime(&temp);
    strftime(tempString, 100, "%Y-%m-%d %H:%M:%S", timePointer);
    printf("\n%s|%s\n", tempString, message);
}

void printTitle(char *title) {
    printf( "\n"
            "===================================================================\n"
            "%s\n"
            "-------------------------------------------------------------------\n", title);
}


char *strStrip(char *string) {
    if(string == NULL) { return NULL; }

    strRStrip(string);
    strLStrip(string);

    return string;
}

char *strRStrip(char *string) {
    int length = strlen(string);
    for(int i = length - 1; i >= 0; i--) {
        if(isspace(string[i])) { string[i] = END; }
        else { return string; }
    }

    return string;
}

char *strLStrip(char *string) {
    char *start = leftSkip(string);
    if(start == string) { return string; }

    int i = 0;
    while(start[i] != END) {
        string[i] = start[i];
        i++;
    }
    string[i] = END;

    return string;
}

char *leftSkip(char *string) {
    int i = 0;
    while( (string[i] != END) && isspace(string[i]) ) {
        i++;
    }

    return &(string[i]);
}

char *strLowerCase(char *string) {  // 문자열을 소문자로 만듬
    for(int i = 0; string[i] != END; i++) {
        string[i] = tolower( (unsigned char) string[i] );
    }

    return string;
}

char *strUpperCase(char *string) {  // 문자열을 대문자로 만듬
    for(int i = 0; string[i] != END; i++) {
        string[i] = toupper( (unsigned char) string[i] );
    }

    return string;
}

bool match(char *str1, char *str2) {  // 문자열 일치 여부 반환
    if(strcmp(str1, str2) == 0) { return true; }

    return false;
}

bool match_n(char *str1, char *str2, int count) {  // 앞에서 n 문자열 일치 여부 반환
    if(strncmp(str1, str2, count) == 0) { return true; }
    
    return false;
}



static String timeString;
char *getTimeString(DateFormat kind) {
    time_t temp;
    struct tm *timePointer;
    
    time(&temp);
    timePointer = localtime(&temp);
    switch(kind) {
    case YYYYMMDD:  // eg) "2023-01-31"
        strftime(timeString, sizeof(timeString), "%Y-%m-%d", timePointer);
        break;
    case YYYYMMDD_HH:  // eg) "2023-01-31 09"
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H", timePointer);
        break;
    case YYYYMMDD_HH_00:  // eg) "2023-01-31 09:00"
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:00", timePointer);
        break;
    case YYYYMMDD_HHmmSS:  // eg) "2023-01-31 09:55:01"
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timePointer);
        break;
    default:  // eg) "2023-01-31 09:55:01"
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timePointer);
        break;
    }

    return timeString;
}


void keyboardBufferCleansing() {
    while(getchar() != '\n');
}
