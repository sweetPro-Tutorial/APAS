#include "ini_parser.h"

int iniParse( char *filename, void *config, KeyHandler keyHandler ) {
    // 검색할 키 목록 준비;  --> API_key 와 interval
    // 파일을 한 줄씩 순회하면서
    FILE *ini = fopen(filename, "r");
    if(ini == NULL) { return FAIL; }
    StringLong line = { 0, };
    while(fgets(line, LINE_SIZE, ini) != NULL) {  // 순회
        strStrip(line);
        // printf(">>> kind:line = %d:%s\n", getKind(line), line);  // test log
        // 만약 키 라인이고,
        if(getKind(line) != KEY) { continue; }
        
        String name;
        String value;
        keyParse(name, value, line);  // 이름과 값 추출
        keyHandler(config, name, value);  // 검색할 키와 일치하면 값을 설정정보에 저장;
    }
   
    fclose(ini);
    return SUCCESS;
}

void keyParse(char *name, char *value, char *line) {
    StringLong tempLine = { 0, };
    char *delimiter = " =";
    char *token;


    strcpy(tempLine, line);
    token = strtok(tempLine, delimiter);  // 이름 추출
    strcpy(name, token);
    strLowerCase(name);
    char *valueStart = leftSkip(strchr(line, '=') + 1);  // 값 추출
    strcpy(value, valueStart);
}


int getKind(char *line) {
    char firstCharacter = line[0];
    if(firstCharacter == ';') { return COMMENT; }
    if(firstCharacter == '[') { return SECTION; }
    if(firstCharacter == END) { return EMPTY_LINE; }
    if(strchr(line, '=') != NULL) { return KEY; }


    return ERROR_LINE;
}
