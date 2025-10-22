#include "clock.h"

#if (USE_FREERTOS == 0)
    clock_time_t clock_time_val ;
    //--------------------------------------------------
    void clock_init(void)
    {
      clock_time_val = 0 ;
    }
    //--------------------------------------------------
    clock_time_t clock_time (void)
    {
      return clock_time_val ;
    }
#endif

