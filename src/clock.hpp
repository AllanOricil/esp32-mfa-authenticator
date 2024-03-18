#ifndef SYNCTIME_H
#define SYNCTIME_H

#include <time.h>
#include <ESP32Time.h>

extern ESP32Time rtc;
void sync_time();

#endif //SYNCTIME_H
