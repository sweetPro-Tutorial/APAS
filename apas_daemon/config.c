#include "config.h"

int keyHandler(void *config, String name, String value) {
    Config *thisConfig = (Config *)config;
    if(match("api_key", name))       { strcpy(thisConfig->api_key, value); }
    else if(match("interval", name)) { thisConfig->interval  = atoi(value); }
    else if(match("threshold", name)){ thisConfig->threshold = atoi(value); }
    else { return FAIL; }

    return SUCCESS;
}

void printConfig(Config *config) {
    printf( "Config=[api_key=%s,interval=%d,threshold=%d]\n",
        config->api_key, config->interval, config->threshold);
}