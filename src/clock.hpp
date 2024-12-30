#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>
#include <esp_log.h>
#include "constants.h"

#ifdef __cplusplus
extern "C"
{
#endif
	void init_clock();
	unsigned long get_elapsed_number_of_time_steps();
	int get_second();
#ifdef __cplusplus
}

#include <ESP32Time.h>

#endif

#endif // CLOCK_H
