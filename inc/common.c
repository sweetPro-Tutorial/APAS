#include "common.h"

void printWithTime(char *message) {
    String tempString;
    time_t temp = time(NULL);
    struct tm *timePointer;
    timePointer = localtime(&temp);
    strftime(tempString, 100, "%Y-%m-%d %H:%M:%S", timePointer);
    printf("\n%s|%s\n", tempString, message);
}
