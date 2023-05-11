#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "collector.h"

int main() {
    printf( "\n"
            "===================================================================\n"
            "데이터 수집\n"
            "-------------------------------------------------------------------\n");
    collector();  // 대기오염정보 수집
}
