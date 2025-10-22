#include "appdefs.h"
#include "can.h"

volatile TestStatus can_test_result  ;
volatile TCANGateType can_gate_type ;

TestStatus can_get_test_result()
{
	return can_test_result ;
}

void can_test( TCANGateType gate_type  )
  {
    CanTxMsg TxMessage;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    uint8_t TransmitMailbox = 0;
    // ��������� ������ �����
    can_gate_type = gate_type ;
    //����� ����� ������� �����
    can_test_result = FALSE ;

    // ��������� ������ GPIO ��������� � CAN1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO |CAN_RCC_APB2Periph_GPIO, ENABLE);

    // ��������� ������ CAN1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;
    // ������������ RX CAN ������
    GPIO_InitStructure.GPIO_Pin = CAN_GPIO_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);
    // ������������ TX CAN ������
    GPIO_InitStructure.GPIO_Pin = CAN_GPIO_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

    // ����������� ������� �� ������ CAN
    GPIO_PinRemapConfig(CAN_GPIO_Remap , ENABLE);


    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // ��������� ������ USB_LP_CAN1_RX0_IRQn ����������� ���������� NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ����� ��������� ������ CAN
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    // ������������� ������ CAN
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Prescaler = 4;//2
    CAN_Init(CAN1, &CAN_InitStructure);

    // ������������� ������� CAN
    CAN_FilterInitStructure.CAN_FilterNumber=0;
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);


    // ���������� ��������� �� ������ ��������� � FIFO #0 ������ CAN1
    CAN_ITConfig(CAN1, CAN_IT_FMP0 , ENABLE);

    if ( can_gate_type == cgtClient )
      {
        //���������� ������ ��� ��������
        TxMessage.StdId = 0x321;
        TxMessage.ExtId = 0x01;
        TxMessage.RTR = CAN_RTR_DATA;
        TxMessage.IDE = CAN_ID_STD;
        TxMessage.DLC = 1;
        TxMessage.Data[0] = 0x55 ;

        // �������� ������
        TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);
        uint32_t i = 0;  // ������ �������� �� ���������� ��������
        while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFFFF))
         {
           i++;
         }

        Delay(100);

       // ��������������� � ���������� CAN ������ (���������� ��� ������� USB ������)
       CAN_DeInit(CAN1);
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE );
     }

  }
