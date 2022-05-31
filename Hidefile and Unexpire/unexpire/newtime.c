#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

// Name: Sihui Qi
// netID: sq77
// RUID: 187002625
// My code for time() goes here

#include <dlfcn.h>
#include <time.h>

time_t (*real_time_func)(time_t *);

time_t time(time_t *seconds) {
    static int bFirstCall = 1;
    real_time_func = dlsym(RTLD_NEXT, "time");
    if(bFirstCall == 1) {
        bFirstCall = 0;
        struct tm tmFakeTime;
        tmFakeTime.tm_year = 2021 - 1900;
        tmFakeTime.tm_mon = 9 - 1;
        tmFakeTime.tm_mday = 1;
        tmFakeTime.tm_hour = 1;
        tmFakeTime.tm_min = 0;
        tmFakeTime.tm_sec = 0;
        tmFakeTime.tm_isdst = -1;
        time_t fake_time = mktime(&tmFakeTime);
        if(seconds != NULL) *seconds = fake_time;
        return fake_time;
    }
    return (*real_time_func)(seconds);
}