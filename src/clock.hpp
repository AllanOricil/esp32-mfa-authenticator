#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>
#include <ESP32Time.h>
#include "constants.h"

void init_clock();
unsigned long get_elapsed_number_of_time_steps();
int get_second();

#endif // CLOCK_H
