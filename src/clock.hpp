#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>
#include <ESP32Time.h>

extern ESP32Time rtc;
void sync_time();

#endif // CLOCK_H
