#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "collector.h"
#include "../inc/common.h"
#include "../inc/ini_parser.h"
#include "config.h"
#include "analyzer.h"

void intro();


int main() {
    Config config = { 0, };
    iniParse("../config/apas.ini", &config, keyHandler);
    printConfig(&config);  // test log

    intro();
    while(true) {
        sleep(1); // 일정시간 대기;
        int currentSecond = time(NULL);  // 현재시간측정;
        // if 특정 주기에 도달했으면 { 반복할 작업 실행; }
        bool intervalFlag = ( (currentSecond % config.interval) == 0 ? true : false );  // 주기 식별용
        if(!intervalFlag) { continue; }
        // 반복할 작업 실행
        printTitle("데이터 수집");
        if(!collector(config.api_key)) { continue; }  // 대기오염정보 수집

        printTitle("데이터 분석 중 (잠시 기다려 주세요)...");
        analyzer(config.threshold);  // 데이터 분석
    }

}



void intro() {
    printf( "\n\n"
            "###################################################################\n"
            "#                                                                 #\n"
            "#                                                                 #\n"
            "#                                                                 #\n"
            "#                 A   P   A   S   -   d a e m o n                 #\n"
            "#                                                                 #\n"
            "#               Air Pollution-info Analysis System                #\n"
            "#                    (대기오염정보분석시스템)                     #\n"
            "#                                                                 #\n"
            "#                                                                 #\n"
            "###################################################################\n\n" );
}
