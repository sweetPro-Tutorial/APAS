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

typedef char StringLong[LINE_SIZE];
typedef char String[ITEM_SIZE];

void printWithTime(char *message);


#endif
