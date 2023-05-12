#include "config_info.h"

int keyHandler(void *config, char *name, char *value) {
    Config *thisConfig = (Config *)config;
    if(match("threshold", name)) { thisConfig->threshold = atoi(value); }
    else { return FAIL; }

    return SUCCESS;
}

void printConfig(Config *config) {
    printf( "Config=[threshold=%d]\n", config->threshold);
}
