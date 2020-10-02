#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/timex.h>


int main(int argc, char* argv[]) {

  struct timespec now_mono;
  struct timespec now_real;
  struct timespec now_tai;
  struct timex details;

  memset(&details, 0, sizeof(struct timex));
  adjtimex(&details);

  clock_gettime(CLOCK_MONOTONIC, &now_mono);
  clock_gettime(CLOCK_TAI, &now_tai);
  clock_gettime(CLOCK_REALTIME, &now_real);

  bool offset_negative = false;
  struct timespec real_tai_offset;
  real_tai_offset.tv_sec = now_real.tv_sec - now_tai.tv_sec;
  real_tai_offset.tv_nsec = now_real.tv_nsec - now_tai.tv_nsec;
  if(real_tai_offset.tv_nsec < 0 && real_tai_offset.tv_sec > 0) {
    real_tai_offset.tv_nsec += 1e9;
    real_tai_offset.tv_sec -= 1;
  }
  if(real_tai_offset.tv_nsec > 0 && real_tai_offset.tv_sec < 0) {
    real_tai_offset.tv_nsec -= 1e9;
    real_tai_offset.tv_sec += 1;
  }
  if(real_tai_offset.tv_nsec < 0) {
    real_tai_offset.tv_nsec = -real_tai_offset.tv_nsec;
    offset_negative = true;
  }
  if(real_tai_offset.tv_sec < 0) {
    real_tai_offset.tv_sec = -real_tai_offset.tv_sec;
    offset_negative = true;
  }
  
  printf("CLOCK_MONOTONIC: %ld.%09ld\n", now_mono.tv_sec, now_mono.tv_nsec);
  printf("CLOCK_REALTIME:  %ld.%09ld\n", now_real.tv_sec, now_real.tv_nsec);
  printf("CLOCK_TAI:       %ld.%09ld (REALTIME %c %ld.%09ld)\n", now_tai.tv_sec, now_tai.tv_nsec, offset_negative ? '+':'-', real_tai_offset.tv_sec, real_tai_offset.tv_nsec);
  
  printf("adjtimex() reported tai_offset: %d\n", details.tai);



  return 0;
}
