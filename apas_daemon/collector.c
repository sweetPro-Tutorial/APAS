#include "collector.h"
#include "response_parser.h"

bool collector(String api_key) {
    if(!requestAirData(api_key)) { return false; }  // 정보연계

    LinkedList list = { NULL, NULL, 0 };
    if(!extractAirInfo(&list)) { return false; };  // 원하는 정보 추출

    // 추출한 대기오염정보(마스터 데이터)와
    AirInfo airInfo = { 0, };
    char *csvString = getBeginNode(&list)->data;
    csvToAirInfo(&airInfo, csvString);
    // 마스터 파일 내의 레코드가 중복되는지 확인
    if(isAlreadyProcessed(airInfo.dataTime)) {
        eraseAllList(&list);
        return false; 
    } 

    // 중복되지 않으면, 마스터 파일에 저장
    saveAirInfo(&list, airInfo.dataTime);

    return true;
}

bool isAlreadyProcessed(String dataTime) {
    FILE *masterFile = openMasterFile("r", dataTime);
    if(masterFile == NULL) { return false; }

    StringLong line = { 0, };
    getLastLine(line, masterFile);
    fclose(masterFile);
    if(match_n(dataTime, line, 13)) { return true; }
    return false; 
}

void saveAirInfo(LinkedList *list, String dataTime) {
    FILE *masterFile = openMasterFile("ab", dataTime);
    if(masterFile == NULL) { return; }

    while(!isEmpty(list)) {
        char *csvString = getBeginNode(list)->data;
        fprintf(masterFile, "\n%s", csvString);  // csv 포맷으로 저장
        popFront(list);
    }
    fclose(masterFile);
}





bool extractAirInfo(LinkedList *list) {
    char *data = loadFile(openFile(RESPONSE_FILENAME, "rb"));  // 응답 메시지 파일 로딩
    if(data == NULL) { return false; }
    // printf("response message=[%s]\n", data);  // test log
    responseParse(list, data);  // 파싱

    free(data);
    if(isEmpty(list)) { return false; }
    return true;    
}

bool requestAirData(String api_key) {
    StringLong requestMessage = { 0, };
    StringLong command = { 0, };

    // 정보연계용 명령어 생성
    sprintf(requestMessage,
            "%s?serviceKey=%s&sidoName=%s&pageNo=1&numOfRows=1000&returnType=json&ver=1.3",
            API, api_key, JEONKOOK_UTF8);
    sprintf(command, "curl \"%s\" > %s", requestMessage, RESPONSE_FILENAME);
    // printf(">>> 명령어: %s\n\n", command);  // test log

    int result = system(command);  // 명령어 실행
    if(result == 0) {  
        printWithTime(" >>> 데이터를 수신했습니다.");
        return true;
    }  
    printWithTime(" >>> 데이터 수신에 실패했습니다.");
    return false;
}