#ifndef __UIP_HANDLER_H__
#define __UIP_HANDLER_H__

//#include "uip.h"



typedef enum
{
  uitRecv = 1 ,
  uiPeriodic = 2
} TUipIrqType ;

//---------------------------------------------------------------------------------
// флаги запросов обработки событий uIP
extern volatile uint32_t uip_irqs ;
// обработчик принятых фреймов
void uip_handler_recv();
// обработчик преодичской активньосои uIP
void uip_handler_periodic();
// запуск uIP стека
void uip_startup();

#endif
