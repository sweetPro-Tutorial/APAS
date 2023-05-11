#include "collector.h"

bool collector() {
    StringLong requestMessage = { 0, };
    StringLong command = { 0, };


    // 정보연계용 명령어 생성
    sprintf(requestMessage,
            "%s?serviceKey=%s&sidoName=%s&pageNo=1&numOfRows=1000&returnType=json&ver=1.3",
            API, MY_KEY, JEONKOOK_UTF8);
    sprintf(command, "curl \"%s\" > %s", requestMessage, RESPONSE_FILENAME);
    // printf(">>> 명령어: %s\n\n", command);  // test log


    int result = system(command);  // 명령어 실행
    if(result == 0) {  
        printf("\n>>> 데이터를 수신했습니다.\n");    
        return true;
    }  
    printf("\n>>> 데이터 수신에 실패했습니다.\n");
    return false;
}
