#ifndef __UIP_HANDLER_H__
#define __UIP_HANDLER_H__

//#include "uip.h"



typedef enum
{
  uitRecv = 1 ,
  uiPeriodic = 2
} TUipIrqType ;

//---------------------------------------------------------------------------------
// ����� �������� ��������� ������� uIP
extern volatile uint32_t uip_irqs ;
// ���������� �������� �������
void uip_handler_recv();
// ���������� ����������� ����������� uIP
void uip_handler_periodic();
// ������ uIP �����
void uip_startup();

#endif
