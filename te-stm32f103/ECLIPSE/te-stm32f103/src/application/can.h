/*
 * adc.h
 *
 *  Created on: 05.11.2009
 *      Author: klen
 */

#ifndef __CAN_H__
#define __CAN_H__

//  ��� ��������� ����
typedef enum
  {
   cgtClient ,   // ���� ��������� ��������, ������� ���, ��������� ������� � ����������� �����
   cgtServer     // ���� ������� ����� ������, ������������� ����� �� ����
  } TCANGateType ;



TestStatus can_get_test_result();
void can_test(TCANGateType gate_type);

#endif /* __CAN_H__ */
