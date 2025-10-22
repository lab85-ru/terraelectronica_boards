#include "debug_led.h"
#include "stm32f10x.h"

static uint8_t debug_led_blink_bits ;

void DebugLedInit()
{
	RCC_APB2PeriphClockCmd(DEBUG_LED_RCC_APB2Periph_GPIO , ENABLE);
    // инициализация вывода LED
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  DEBUG_LED_PIN_D10 | DEBUG_LED_PIN_D11 | DEBUG_LED_PIN_D12;
    GPIO_InitStructure.GPIO_Mode = DEBUG_LED_GPIO_MODE;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_LED_PORT, &GPIO_InitStructure);
}
//------------------------------------------------------------
void DebugLedToggleD10()
{
  if ( DEBUG_LED_PORT->IDR & DEBUG_LED_PIN_D10 )
    DEBUG_LED_PORT->BRR =  DEBUG_LED_PIN_D10 ;
  else
    DEBUG_LED_PORT->BSRR = DEBUG_LED_PIN_D10 ;
}
//------------------------------------------------------------
void DebugLedToggleD11()
{
  if ( DEBUG_LED_PORT->IDR & DEBUG_LED_PIN_D11 )
    DEBUG_LED_PORT->BRR =  DEBUG_LED_PIN_D11 ;
  else
    DEBUG_LED_PORT->BSRR = DEBUG_LED_PIN_D11 ;
}
//------------------------------------------------------------
void DebugLedToggleD12()
{
  if ( DEBUG_LED_PORT->IDR & DEBUG_LED_PIN_D12 )
    DEBUG_LED_PORT->BRR =  DEBUG_LED_PIN_D12 ;
  else
    DEBUG_LED_PORT->BSRR = DEBUG_LED_PIN_D12 ;
}
//------------------------------------------------------------

/* состояния индикации
0 - негорит
1 - горит
х - моргает

state bits
 CAN ADC SD
  2   1   0

          state    D12    D11    D10
ничего      0        0      0      x
 SD         1        0      1      x
 ADC        2        1      0      x
 ADC+SD     3        1      1      x
 CAN        4        0      x      x
 CAN+SD     5        1      x      x
 CAN+ADC    6        x      0      x
CAN+SD+ADC  7        x      x      x
*/


void DebugLedInitIndication( TestStatus can_state , TestStatus adc_state , TestStatus sd_state )
{
	uint8_t state = (can_state << 2) | (adc_state << 1) | sd_state ;
	debug_led_blink_bits = 0 ;
	switch( state )
	{
	   case 1 :
		   DebugLedToggleD11();
		   break ;
	   case 2 :
		   DebugLedToggleD12();
		   break ;
	   case 3 :
	   	   DebugLedToggleD11();
	   	   DebugLedToggleD12();
	   	   break ;
	   case 4 :
		   debug_led_blink_bits = 1 ;
	       break ;
	   case 5 :
		   DebugLedToggleD12();
		   debug_led_blink_bits = 1 ;
		   break ;
	   case 6 :
		   debug_led_blink_bits = 2 ;
		   break ;
	   case 7 :
	   	   debug_led_blink_bits = 3 ;
		   break ;
	   default : break ;
	}
}
//------------------------------------------------------------
void DebugLedsToggle( )
{
  DebugLedToggleD10();
  if ( debug_led_blink_bits & 0x1 ) DebugLedToggleD11();
  if ( debug_led_blink_bits & 0x2 ) DebugLedToggleD12();
}
//------------------------------------------------------------

