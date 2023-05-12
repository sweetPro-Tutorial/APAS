#include "../inc/common.h"
#include "../inc/air_info.h"
#include "../inc/ini_parser.h"
#include "config_info.h"
#include "../inc/statistics_info.h"

#define MENU_SIZE        4

typedef struct {
    String date;
    String time;
    int    sidoCode;
} Conditions;

static String menuTable[MENU_SIZE] = { 
    "대기오염 현황 조회 (시도/일/시간별)",
    "알람발생 현황 조회 (시도별)",
    "대기오염 통계 조회 (시도/일별)",
    "프로그램 종료",
};

void programTitle();  // 프로그램 intro
void printMenuTable();  // 메인 메뉴 출력
void inputMenu(int *menu);  // 메인 메뉴 사용자 입력받기
bool isValidMenu(int menu);  // 적절한 메뉴인지 확인

void searchAirInfo();  // 대기오염 현황 조회 (시도/일/시간별)
void searchAlarm(int threshold);  // 알람발생 현황 조회 (시도별)
void searchStatistics();  // 대기오염 통계 조회 (시도/일별)

void listupAirInfoPerCity(Conditions *conditions);
void listupAlarmInfoPerCity(Conditions *conditions, int threshold);
void listupStatisticsPerCity(Conditions *conditions);

void inputDate(String date);  // 날짜 입력
void inputTime(String time);  // 시간 입력 (허용값: 00~23)
void inputSido(int *sidoCode);  // 시도 코드 입력
void inputMenu(int *menu);

void printSidoTable();

bool isValidDate(String date);  // 입력값이 정상인지 확인
bool isValidTime(String time);  // 입력값이 정상인지 확인
bool isValidSido(int sidoCode);  // 입력값이 정상인지 확인
bool isValidMenu(int menu);
void makeTwoLetterTime(String time);  // 9 -> 09
void makeDashedDate(String date);  // YYYYMMDD --> YYYY-MM-DD

void pressAnyKeyToContinue();

int main() {
    printTitle("환경 설정정보 업로딩...");
    Config config = { 0, };
    iniParse("../config/apas.ini", &config, keyHandler);  // 환경 설정정보 로드
    printConfig(&config);  // test log

    int menu;
    int threshold = 3;
    while(true) {  // 루프를 돌면서
        programTitle();
        sleep(1);
        inputMenu(&menu);  // 메인 메뉴에서 메뉴를 선택
        switch(menu) {  // 선택한 기능 호출
        case 1: searchAirInfo(); break;
        case 2: searchAlarm(threshold); break;
        case 3: searchStatistics(); break;
        case 4: exit(0);
        default: break;
        }
    }
}

void inputMenu(int *menu) {
    while(true) {
        printTitle("M  E  N  U");
        printMenuTable();
        printf("\n메뉴를 선택하세요 (예: 1): ");
        fflush(stdout);  // for windows
        scanf("%d", menu);
        keyboardBufferCleansing();
        if(isValidMenu(*menu)) { return; }  // 정상적인 선택인지 확인

        printf("==> 잘못된 선택입니다. 다시 선택하세요.\n");
    }
}

bool isValidMenu(int menu) {
    if(menu < 1 || menu > MENU_SIZE) { return false; }
    
    return true;
}

void printMenuTable() {
    for(int i = 0; i < MENU_SIZE; i++) {
        printf("%5d. %s\n", i + 1, menuTable[i]);
    }
}

void programTitle() {
    printf( "\n\n"
            "###################################################################\n"
            "#                                                                 #\n"
            "#                                                                 #\n"
            "#                                                                 #\n"
            "#               A   P   A   S   -   o p e r a t o r               #\n"
            "#                                                                 #\n"
            "#               Air Pollution-info Analysis System                #\n"
            "#                    (대기오염정보분석시스템)                     #\n"
            "#                                                                 #\n"
            "#                                                                 #\n"
            "###################################################################\n\n" );
}

void searchAirInfo() {
    // 검색 조건 입력
    printTitle(menuTable[1]);
    printf("검색조건(시도 번호, 날짜, 시간 범위)을 입력하세요.\n");
    Conditions conditions = { 0, };
    inputSido(&conditions.sidoCode);
    inputDate(conditions.date);
    inputTime(conditions.time);

    // 시도별/일별/시간별 대기오염 현황 검색결과 출력
    listupAirInfoPerCity(&conditions);
    printf("\n");
    pressAnyKeyToContinue();
}

void pressAnyKeyToContinue() {
    printf(">>> 메인 메뉴로 돌아가려면 Enter 키를 누르세요: ");
    
    fflush(stdout);
    while(true) {
        char c = getchar();
        if(c == '\n') { return; }
    }
}

void listupAirInfoPerCity(Conditions *conditions) {
    char *data = loadFile(openMasterFile("r", conditions->date));
    if(data == NULL) { data = calloc(1, 1); }  // 에러방지: 빈 문자열을 할당("")

    // 헤더: 검색결과용 필드명 출력
    int sidoCode = conditions->sidoCode;
    String dataTime = { 0, };
    sprintf(dataTime, "%s %s", conditions->date, conditions->time);
    printTitle("검색 결과");
    printf( "\n"
            "[%s] 측정시각: %s:00\n"
            "       등    급: 1 좋음, 2 보통, 3 나쁨, 4 매우나쁨, - 통신장애\n"
            "\n\n"
            "  #    등급(농도):미세먼지     등급(농도):초미세먼지     측정소\n"
            "-------------------------------------------------------------------\n",
        sidoCodeToName(sidoCode), dataTime); 
    
    // 바디: 검색된 레코드 출력
    AirInfo airInfo = { 0, };
    int count = 0;
    char *delimiter = "\n";
    char *token;
    char *savePointer;
    token = strtok_r(data, delimiter, &savePointer);
    while(token != NULL) {  // 마스터 데이터 순회
        // printf(">>> token : %s\n", token);  // test log
        csvToAirInfo(&airInfo, token);
        bool selected;
        if(sidoCode == NATION) {  // 전국
            selected = match(airInfo.dataTime, dataTime); 
        } else { 
            selected = match(airInfo.dataTime, dataTime) && (airInfo.sidoCode == sidoCode);
        }
        if(selected) {
            count++;
            printf("%3d    %1s (%3s ㎍/㎥ )          %1s (%3s ㎍/㎥ )            %-s\n", 
                count, 
                match(airInfo.pm10Grade1h, "null") ? "-" : airInfo.pm10Grade1h,
                airInfo.pm10Value, 
                match(airInfo.pm25Grade1h, "null") ? "-" : airInfo.pm25Grade1h,
                airInfo.pm25Value, 
                airInfo.stationName);
        }

        token = strtok_r(NULL, delimiter, &savePointer);
    }
    printf("\nRecord count = %d\n", count);

    free(data);
}

void inputSido(int *sidoCode) {
    while(true) {
        printf("\n\n");
        printSidoTable();
        printf("\n검색할 [시도 번호]를 입력하세요 (예: 17): ");
        fflush(stdout);  // for windows
        scanf("%2d", sidoCode);
        keyboardBufferCleansing();
        if(isValidSido(*sidoCode)) { 
            (*sidoCode)--;
            return; 
        }
        printf("==> 잘못된 선택입니다. 다시 선택하세요.\n");
    }
}

void inputDate(String date) {
    time_t temp;
    struct tm *timePointer;    
    time(&temp);
    timePointer = localtime(&temp);
    String timeString;
    strftime(timeString, sizeof(timeString), "%Y%m%d", timePointer);

    while(true) {
        printf("\n검색할 [날짜]를 입력하세요 (예: %s): ", timeString);
        fflush(stdout);  // for windows
        scanf("%8s", date);
        keyboardBufferCleansing();
        if(isValidDate(date)) { 
            makeDashedDate(date);
            return; 
        }
        printf("==> 잘못된 날짜입니다. 다시 입력하세요.\n");
    }
}

void inputTime(String time) { 
    while(true) {
        printf("\n검색할 [시간]을 입력하세요 (예: 09): ");
        fflush(stdout);  // for windows
        scanf("%2s", time);
        keyboardBufferCleansing();
        if(isValidTime(time)) { 
            makeTwoLetterTime(time);  // ex) 9 -> 09
            return; 
        }
        printf("==> 잘못된 시간입니다. 다시 입력하세요.\n");
    }
}

bool isValidSido(int sidoCode) {
    if(sidoCode < 1 || sidoCode > SIDO_TABLE_SIZE) { return false; }
    
    return true;
}

bool isValidDate(String date) {
    if(strlen(date) != 8) { return false; }
    if(strspn(date, NUMBER) == 0) { return false; }

    return true;
}

bool isValidTime(String time) {
    if(strspn(time, NUMBER) == 0) { return false; }
    int value = atoi(time);
    if(value > 23 || value < 0) { return false; }  // 허용: 0 ~ 23

    return true;
}

void printSidoTable() {
    // for(int i = 0; i < SIDO_TABLE_SIZE; i++) {  // 횡방향으로 출력시
    //     if( (i % 3) == 0) { printf("\n"); }
    //     printf("%5d. %s", i + 1, sidoCodeToName(i));
    // }  

    int columns = 3;
    int rest = SIDO_TABLE_SIZE % columns;
    int rows = rest ? (SIDO_TABLE_SIZE / columns) + 1 : (SIDO_TABLE_SIZE / columns); 
    for(int i = 0; i < rows; i++) {  // 종방향으로 출력시
        for(int j = 0; j < columns; j++) {
            int index = i + (j * rows);
            if(index < SIDO_TABLE_SIZE) { 
                printf("%5d. %s", index + 1, sidoCodeToName(index)); 
            }
        }
        printf("\n");
    }
}

void makeDashedDate(String date) {  // YYYYMMDD --> YYYY-MM-DD
    String temp;
    strcpy(temp, date);
    sprintf(date, "%.4s-%.2s-%.2s", temp, &temp[4], &temp[6]);
}

void makeTwoLetterTime(String time) {
    if(strlen(time) == 2) { return; }
    int number = atoi(time);
    sprintf(time, "%02d", number);
}





void searchAlarm(int threshold) {
    printTitle(menuTable[2]);
    printf("검색조건(시도 번호)을 입력하세요.\n");
    Conditions conditions = { 0, };
    inputSido(&conditions.sidoCode);
    // printf(">>> Conditions=[date=%s,time=%s,sidoCode=%d]\n",
    //     conditions.date, conditions.time, conditions.sidoCode);  // test log

    listupAlarmInfoPerCity(&conditions, threshold);
    printf("\n");
    pressAnyKeyToContinue();
}

static String gradeName[5] = { "", "좋음", "보통", "나쁨", "매우 나쁨" };

void listupAlarmInfoPerCity(Conditions *conditions, int threshold) {
    char *data = loadFile(openAlarmFile("r"));
    if(data == NULL) { data = calloc(1, 1); }  // 에러방지: 빈 문자열을 할당("")

    // 헤더: 검색결과용 필드명 출력
    int sidoCode = conditions->sidoCode;
    printTitle("검색 결과");
    printf( "\n"
            "[%s] 현재시각: %s\n"
            "       대기 상태가 '%s' 이상인 장소입니다 (O 대상, X 비대상).\n"
            "\n\n"
            "  #    미세먼지 농도           초미세먼지 농도           측정소\n"
            "-------------------------------------------------------------------\n",
        sidoCodeToName(sidoCode), getTimeString(YYYYMMDD_HHmmSS), 
        gradeName[threshold]); 
    
    // 바디: 검색된 레코드 출력
    AirInfo airInfo = { 0, };
    int count = 0;
    char *delimiter = "\n";
    char *token;
    char *savePointer;
    token = strtok_r(data, delimiter, &savePointer);
    while(token != NULL) {  // 알람 데이터 순회
        // printf(">>> token : %s\n", token);  // test log
        csvToAirInfo(&airInfo, token);
        bool selected;
        if(sidoCode == NATION) { selected = true; }
        else { selected = (airInfo.sidoCode == sidoCode); } 
        if(selected) {
            count++;
            printf("%3d    %3s ㎍/㎥ (%c)          %3s ㎍/㎥ (%c)              %-s\n", 
                count, airInfo.pm10Value, 
                atoi(airInfo.pm10Grade1h) < threshold ? 'X' : 'O',
                airInfo.pm25Value,
                atoi(airInfo.pm25Grade1h) < threshold ? 'X' : 'O',
                airInfo.stationName);
        }

        token = strtok_r(NULL, delimiter, &savePointer);
    }
    printf("\nRecord count = %d\n", count);

    free(data);
}

void searchStatistics() {
    printTitle(menuTable[3]);
    printf("검색조건(시도 번호, 날짜)을 입력하세요.\n");
    Conditions conditions = { 0, };
    inputSido(&conditions.sidoCode);
    inputDate(conditions.date);

    listupStatisticsPerCity(&conditions);
    printf("\n");
    pressAnyKeyToContinue();
}

void listupStatisticsPerCity(Conditions *conditions) {
    char *data = loadFile(openStatisticsFile("r", conditions->date));
    if(data == NULL) { data = calloc(1, 1); }  // 에러방지: 빈 문자열을 할당("")

    // 헤더: 검색결과용 필드명 출력
    int sidoCode = conditions->sidoCode;
    printTitle("검색 결과");
    printf( "\n"
            "[%s] 일평균 통계       날    짜: %s\n"
            "       등    급: 1 좋음, 2 보통, 3 나쁨, 4 매우나쁨, - 통신장애\n"
            "\n\n"
            "       미세먼지                초미세먼지\n"
            "  #    평균등급(평균농도)      평균등급(평균농도)        측정소\n"
            "-------------------------------------------------------------------\n",
        sidoCodeToName(sidoCode), conditions->date); 
    
    // 바디: 검색된 레코드 출력
    StatisticsInfo statisticsInfo = { 0, };
    int count = 0;
    char *delimiter = "\n";
    char *token;
    char *savePointer;
    token = strtok_r(data, delimiter, &savePointer);
    while(token != NULL) {  // 마스터 데이터 순회
        // printf(">>> token : %s\n", token);  // test log
        csvToStatisticsInfo(&statisticsInfo, token);
        bool selected;
        if(sidoCode == NATION) { selected = true; }
        else { selected = (statisticsInfo.sidoCode == sidoCode); } 
        if(selected) {
            count++;
            printf("%3d    %3.1f ( %5.1f ㎍/㎥ )     %3.1f ( %5.1f ㎍/㎥ )       %-s\n", 
                count, 
                statisticsInfo.pm10Grade1h_average,
                statisticsInfo.pm10Value_average,
                statisticsInfo.pm25Grade1h_average,
                statisticsInfo.pm25Value_average,
                statisticsInfo.stationName);
        }

        token = strtok_r(NULL, delimiter, &savePointer);
    }
    printf("\nRecord count = %d\n", count);
    
    free(data);
}

