#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "collector.h"

int main() {
    while(true) {
        sleep(1); // 일정시간 대기;
        int currentSecond = time(NULL);  // 현재시간측정;
        // if 특정 주기에 도달했으면 { 반복할 작업 실행; }
        bool intervalFlag = ( (currentSecond % 10) == 0 ? true : false );  // 주기 식별용
        if(!intervalFlag) { continue; }
        // 반복할 작업 실행
        printf( "\n"
                "===================================================================\n"
                "데이터 수집\n"
                "-------------------------------------------------------------------\n");
        collector();  // 대기오염정보 수집
    }
}
