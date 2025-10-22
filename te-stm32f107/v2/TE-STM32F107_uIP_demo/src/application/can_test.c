#include "appdefs.h"
#include "can_test.h"
#include "stm32f10x_can.h"

//------------------------------------------------------------------------------
volatile uint32_t can_test_result ;
//------------------------------------------------------------------------------
void can_deinit(CAN_TypeDef* CANx)
{
    // сброс регистров модуля CAN
    CAN_DeInit(CANx);

	// активация портов GPIO связанных с CAN1
	if (CANx==CAN1)
	   RCC_APB2PeriphClockCmd(CAN1_RCC_APBPeriph_AFIO |CAN1_RCC_APBPeriph_GPIO, DISABLE);
	else
	   RCC_APB2PeriphClockCmd(CAN2_RCC_APBPeriph_AFIO |CAN2_RCC_APBPeriph_GPIO, DISABLE);


    // активация модуля CAN1
    if (CANx==CAN1)
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
    else
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);




    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // установка канала CANx_RX0_IRQn контроллера прервыаний NVIC
    if (CANx==CAN1)
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    else
    	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    // разрешение прерыания по приему сообщения в FIFO #0 модуля CAN1
    CAN_ITConfig(CANx, CAN_IT_FMP0 , DISABLE);

}

void can_init(CAN_TypeDef* CANx)
  {
    // активация портов GPIO связанных с CAN1
	if (CANx==CAN1)
	   RCC_APB2PeriphClockCmd(CAN1_RCC_APBPeriph_AFIO |CAN1_RCC_APBPeriph_GPIO, ENABLE);
	else
	   RCC_APB2PeriphClockCmd(CAN2_RCC_APBPeriph_AFIO |CAN2_RCC_APBPeriph_GPIO, ENABLE);


    GPIO_InitTypeDef  GPIO_InitStructure;

    // конфигурация RX CAN вывода
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    if (CANx==CAN1)
     {
       GPIO_InitStructure.GPIO_Pin = CAN1_RX_GPIO_PIN;
       GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);
     }
    else
     {
       GPIO_InitStructure.GPIO_Pin = CAN2_RX_GPIO_PIN;
       GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);
     }

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // конфигурация TX CAN вывода
    if (CANx==CAN1)
      {
        GPIO_InitStructure.GPIO_Pin = CAN1_TX_GPIO_PIN;
        GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);
        // отображение выводов на модуль CAN
        GPIO_PinRemapConfig(CAN1_GPIO_Remap , ENABLE);
      }
    else
      {
    	GPIO_InitStructure.GPIO_Pin = CAN2_TX_GPIO_PIN;
    	GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);
    	GPIO_PinRemapConfig(CAN2_GPIO_Remap , ENABLE);
      }

    // активация модуля CAN1
    if (CANx==CAN1)
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
    else
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);




    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // установка канала CANx_RX0_IRQn контроллера прервыаний NVIC
    if (CANx==CAN1)
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    else
    	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // сброс регистров модуля CAN
    CAN_DeInit(CANx);

    CAN_InitTypeDef   CAN_InitStructure;
    CAN_StructInit(&CAN_InitStructure);
    // инициалищация модуля CAN
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal ;
    //CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack ;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Prescaler = 4;//2
    CAN_Init(CANx, &CAN_InitStructure);

    // инициализация фильтра CAN
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    if (CANx==CAN1)
      CAN_FilterInitStructure.CAN_FilterNumber=0;
    else
      CAN_FilterInitStructure.CAN_FilterNumber=14;

    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    // разрешение прерыания по приему сообщения в FIFO #0 модуля CAN1
    CAN_ITConfig(CANx, CAN_IT_FMP0 , ENABLE);

  }
//-------------------------------------------------------------------------------
uint32_t can_tx(CAN_TypeDef* CANx , uint8_t* data , uint32_t size)
{
	CanTxMsg TxMessage;
    uint8_t TransmitMailbox = 0;

	//подготовка пакета для передачи
    TxMessage.StdId = 0x321;
    TxMessage.ExtId = 0x01;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 1;
    TxMessage.Data[0] = 0x55 ;

    // передача пакета
    TransmitMailbox=CAN_Transmit(CANx, &TxMessage);
    uint32_t i = 0xffff;  // таймер таймаута по завершении передачи
    while( (CAN_TransmitStatus(CANx, TransmitMailbox) != CANTXOK) && i )
      {
        i--;
      }
    if ( !i ) return 0xFFFFFFFF ;
    return size ;
  }
//--------------------------------------------------------------------------------

uint32_t can_test()
{
	uint8_t can_data[8] ;
	can_init(CAN1) ;
	can_init(CAN2) ;
    can_data[0]=0x55 ;
	can_test_result = 0  ;
    can_tx(CAN1 , can_data , 0) ;
	can_tx(CAN2 , can_data , 0) ;
    can_deinit(CAN1) ;
	can_deinit(CAN2) ;
	uint32_t i = 0xffff;  // таймер таймаута по приему
	while( (can_test_result != 2)  && (i--) ) ;
    return  (can_test_result == 2) ;
}
