// chrono.h
//
// A small library to measure time in programs
//
// by W.Zola (2017)

#include "chrono.h"

void chronoReset(chronometer_t *chrono) {
  chrono->xtotal_ns = 0;
  chrono->xn_events = 0;
}

void chronoStart(chronometer_t *chrono) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &(chrono->xadd_time1) );
}

long long chronoGetTotal(chronometer_t *chrono) {
  return chrono->xtotal_ns;
}

// decrement (adjust) a number of ns from chonometer
long long chronoDecrease(chronometer_t *chrono, long long decrease_time_ns) {
  chrono->xtotal_ns -= decrease_time_ns;
  return chrono->xtotal_ns;
}
  
long long chronoGetCount(chronometer_t *chrono) {
  return chrono->xn_events;
}

void chronoStop(chronometer_t *chrono) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &(chrono->xadd_time2) );
  
  long long ns1 = chrono->xadd_time1.tv_sec*1000*1000*1000 + 
                  chrono->xadd_time1.tv_nsec;
  long long ns2 = chrono->xadd_time2.tv_sec*1000*1000*1000 + 
                  chrono->xadd_time2.tv_nsec;
  long long deltat_ns = ns2 - ns1;
      
  chrono->xtotal_ns += deltat_ns;
  chrono->xn_events++;
}

void chronoReportTime(chronometer_t *chrono, const char *s) {   
  printf("\n%s deltaT(ns): %lld ns for %ld ops \n"
         "        ==> each op takes %lld ns\n",
         s, chrono->xtotal_ns, chrono->xn_events, 
         chrono->xtotal_ns/chrono->xn_events );
}

void chronoReportTimeInLoop(chronometer_t *chrono, char *s, int loop_count) {
  printf("\n%s deltaT(ns): %lld ns for %ld ops \n"
         "        ==> each op takes %lld ns\n",
         s, chrono->xtotal_ns, chrono->xn_events*loop_count, 
         chrono->xtotal_ns/(chrono->xn_events*loop_count) );
}