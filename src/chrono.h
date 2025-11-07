// chrono.h
//
// A small library to measure time in programs
//
// by W.Zola (2017)

#ifndef _CHRONO_
#define _CHRONO_

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/time.h>     /* struct timeval definition           */
#include <unistd.h>       /* declaration of gettimeofday()       */

#include <time.h>


typedef struct {
  struct timespec xadd_time1, xadd_time2;
  long long xtotal_ns;
  long xn_events;  
} chronometer_t;
 

void chronoReset(chronometer_t *chrono);

void chronoStart(chronometer_t *chrono);

long long chronoGetTotal(chronometer_t *chrono);

// decrement (adjust) a number of ns from chonometer
long long chronoDecrease(chronometer_t *chrono, long long decrease_time_ns);
  
long long chronoGetCount(chronometer_t *chrono);

void chronoStop(chronometer_t *chrono);

void chronoReportTime(chronometer_t *chrono, const char *s);

void chronoReportTimeInLoop(chronometer_t *chrono, char *s, int loop_count);

#endif