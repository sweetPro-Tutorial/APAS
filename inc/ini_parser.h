#ifndef _INI_PARSER_H_
#define _INI_PARSER_H_


#include "common.h"

typedef enum { COMMENT, SECTION, KEY, EMPTY_LINE, ERROR_LINE } IniKind;

typedef int (*KeyHandler)(void *config, char *name, char *value);

// ini 파일을 파싱: 성공 시 0, 실패 시 -1 반환
int iniParse( char *filename, void *config, KeyHandler keyHandler );

// 이 라인이 ini 포맷 중에서 무엇인지 판단
int getKind(char *line);

// 이름과 값 추출 (키 라인 예시: name = value)
void keyParse(char *name, char *value, char *line);


#endif