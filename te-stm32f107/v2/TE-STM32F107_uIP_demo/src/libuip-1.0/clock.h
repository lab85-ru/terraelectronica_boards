#ifndef __CLOCK_H__
#define __CLOCK_H__

#if (USE_FREERTOS==1)
  #include "freertos"
  typedef portTickType clock_time_t;
  #define clock_time() xTaskGetTickCount()
  #define clock_init()
#else
  typedef unsigned long clock_time_t;
  extern clock_time_t clock_time_val ;
  void clock_init(void);
  clock_time_t clock_time(void);
#endif

  #endif /* __CLOCK_H__ */
