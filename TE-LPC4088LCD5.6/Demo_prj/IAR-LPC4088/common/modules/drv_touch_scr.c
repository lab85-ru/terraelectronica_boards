/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_touch_scr.c
 *    Description : Touch screen driver module
 *
 *    History :
 *    1. Date        : August, 8 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  Touch screen sampling algorithm
 *   1. Set Y1, Y2 to output 1, Set X1 - ADC input (ch 1), X2 input with pull-down
 *   2. Wait setup delay
 *   3. Check X2 state - if high the screen is touched set Y0 to low and wait
 *                        setup delay
 *                       else wait until interrupt by rising edge from X2
 *      When interrupt arise Y0 to low and wait setup delay
 *   4. Star conversion
 *   5. After TS_SAMPLES change Y1 high Y2 low and result calculate  1023 - ADC
 *   6. After TS_SAMPLES change Y1 to ADC  input Y2 input without pulls X1 and X2
 *      output in low and high level
 *   7. Same like X
 *   8. Apply sample delay and return back to poin 1
 *
 *    $Revision: 57747 $
 **************************************************************************/
#include "LPC407x_8x.h"
#include "board.h"   
#include "drv_touch_scr.h"
#include "assert.h"

typedef enum _TouchScrState_t
{
  TS_INTR_SETUP_DLY = 0, TS_WAIT_FOR_TOUCH,
  TS_X1_SETUP_DLY, TS_X1_MEASURE,
  TS_X2_SETUP_DLY, TS_X2_MEASURE,
  TS_Y1_SETUP_DLY, TS_Y1_MEASURE,
  TS_Y2_SETUP_DLY, TS_Y2_MEASURE,
} TouchScrState_t;

#define LEFT_UP_X      80 * TS_SAMPLES * 2
#define RIGHT_UP_X    945 * TS_SAMPLES * 2

#define LEFT_UP_Y     192 * TS_SAMPLES * 2
#define RIGHT_UP_Y    818 * TS_SAMPLES * 2


static volatile uint32_t Touch;
static volatile uint16_t  X,Y;
static volatile uint16_t  X_temp,Y_temp;
static volatile uint32_t Touch_temp;

static volatile TouchScrState_t State;
static volatile uint32_t Samples;

/*************************************************************************
 * Function Name: TimerIntr_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sample timer interrupt handler
 *
 *************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = (1<<0);  /* clear pending MR0INT*/
  LPC_TIM1->TCR = (1<<1); /* */
  switch(State)
  {
  case TS_X1_SETUP_DLY:
  case TS_X2_SETUP_DLY:
  case TS_Y1_SETUP_DLY:
  case TS_Y2_SETUP_DLY:
    ++State;
    LPC_ADC->CR |= (1<<24); 
    break;
  case TS_INTR_SETUP_DLY:
    ++State;
    TS_X2_INTR_CLR = TS_X2_MASK;
    if(0 == (TS_X2_FIO & TS_X2_MASK))
    {
      Touch_temp = Touch = FALSE;
      TS_X2_INTR_R |= TS_X2_MASK;
    }
    else
    {
      // Update X and Y
      if(Touch_temp)
      {
        X = X_temp;
        Y = Y_temp;
      }

      Touch = Touch_temp;

      // Y1 = 0, Y2 = 1;
      TS_Y1_FCLR = TS_Y1_MASK;
      // Disable X2 pull down
      TS_X2_IOCON &= ~(3<<3);
      // Reset sample counter
      Samples = 0;
      // Clear accumulators
      X_temp = Y_temp = 0;
      // Init setup delay
      if(Touch)
      {
        LPC_TIM1->MR0 = TS_SETUP_DLY;
      }
      else
      {
       LPC_TIM1->MR0 = TS_INIT_DLY;
      }
      State = TS_X1_SETUP_DLY;
      LPC_TIM1->TCR = 1;
    }
    break;
  default:
    assert(0);
    break;
  }

  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: OnTouchIntr_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: On touch interrupt handler
 *
 *************************************************************************/
void GPIO_IRQHandler (void)
{
  // Disable and clear interrupt
  TS_X2_INTR_R  &= ~TS_X2_MASK;
  TS_X2_INTR_CLR =  TS_X2_MASK;
  // Init ACD measure setup delay
  if(TS_WAIT_FOR_TOUCH == State)
  {
    // Y1 = 0, Y2 = 1;
    TS_Y1_FCLR = TS_Y1_MASK;
    // Disable X2 pull down
    TS_X2_IOCON &= ~(3<<3);
    // Reset sample counter
    Samples = 0;
    // Clear accumulators
    X_temp = Y_temp = 0;
    // Init setup delay
    if(Touch)
    {
      LPC_TIM1->MR0 = TS_SETUP_DLY;
    }
    else
    {
      LPC_TIM1->MR0 = TS_INIT_DLY;
    }
    State = TS_X1_SETUP_DLY;
    LPC_TIM1->TCR = 1;
  }
  else
  {
    assert(0);
  }
  /*Data synchronization*/
  __DSB();
}


/*************************************************************************
 * Function Name: ADC_Intr_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: End conversion interrupt handler
 *
 *************************************************************************/
void ADC_IRQHandler (void)
{
uint32_t Data;
  LPC_ADC->CR &= ~(7<<24);
  Data = (LPC_ADC->GDR & (0x0000FFF0))>>6;  /*10 - bit result*/
  
  switch(State)
  {
  case TS_X1_MEASURE:
    Y_temp += Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_X2_SETUP_DLY;
      // Y2 = 0, Y1 = 1;
      TS_Y2_FCLR = TS_Y2_MASK;
      TS_Y1_FSET = TS_Y1_MASK;
      // Init setup delay
      LPC_TIM1->MR0 = TS_SETUP_DLY;
      LPC_TIM1->TCR = 1;
    }
    else
    {
      LPC_ADC->CR |= (1<<24);
    }
    break;
  case TS_X2_MEASURE:
    Y_temp += 1023UL - Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_Y1_SETUP_DLY;

      // X1 = 0, X2 = 1;
      TS_X1_FCLR  = TS_X1_MASK;
      TS_X2_FSET  = TS_X2_MASK;
      TS_X1_FDIR |= TS_X1_MASK;
      TS_X2_FDIR |= TS_X2_MASK;
      TS_X1_IOCON   = 0x1A0;

      // Y1 - ADC Ch0, Y2 input
      TS_Y1_FDIR &= ~TS_Y1_MASK;
      TS_Y2_FDIR &= ~TS_Y2_MASK;
      TS_Y1_IOCON   = 0x101;            // assign to ADC0 Ch0
      LPC_ADC->CR  &= ~(0xFF<<0);       
      LPC_ADC->CR  |= (1<<0);          // select Ch0

      // Init setup delay
      LPC_TIM1->MR0 = TS_SETUP_DLY;
      LPC_TIM1->TCR = 1;
    }
    else
    {
      LPC_ADC->CR |= (1<<24);
    }
    break;
  case TS_Y1_MEASURE:
    X_temp += 1023UL - Data;
    if(++Samples >= TS_SAMPLES)
    {
      Samples = 0;
      State = TS_Y2_SETUP_DLY;
      // X2 = 0, X1 = 1;
      TS_X2_FCLR = TS_X2_MASK;
      TS_X1_FSET = TS_X1_MASK;
      // Init setup delay
      LPC_TIM1->MR0 = TS_SETUP_DLY;
      LPC_TIM1->TCR = 1;
    }
    else
    {
      LPC_ADC->CR |= (1<<24);
    }
    break;
  case TS_Y2_MEASURE:
    X_temp += Data;
    if(++Samples >= TS_SAMPLES)
    {
      State = TS_INTR_SETUP_DLY;

      // Y1 = 1, Y2 = 1;
      TS_Y1_FSET  = TS_Y1_MASK;
      TS_Y2_FSET  = TS_Y2_MASK;
      TS_Y1_FDIR |= TS_Y1_MASK;
      TS_Y2_FDIR |= TS_Y2_MASK;
      TS_Y1_IOCON   = 0x1A0;

      // X1 - ADC Ch1, X2 input with pull down
      TS_X1_FDIR &= ~TS_X1_MASK;
      TS_X2_FDIR &= ~TS_X2_MASK;
      TS_X1_IOCON   = 0x101;          // assign to ADC0 Ch1
      TS_X2_IOCON  = 0x28;            // enable pull-down
      LPC_ADC->CR  &= ~(0xFF<<0);       
      LPC_ADC->CR  |= (1<<1);          // select Ch1

      // Init setup delay
      LPC_TIM1->MR0 = TS_SAMPLE_DLY;
      LPC_TIM1->TCR = 1;
      Touch_temp = TRUE;
    }
    else
    {
      LPC_ADC->CR |= (1<<24);
    }
    break;
  default:
    assert(0);
  }
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Touch screen
 *
 *************************************************************************/
void TouchScrInit (void)
{
  /* Init variables */
  Touch_temp = Touch = FALSE;
  X = Y = 0;
  State = TS_INTR_SETUP_DLY;

  /* Init GPIOs */
  
  TS_X1_IOCON   = 0x101;  /* ADC0 Ch1 disable pulls */
  TS_X2_IOCON   = 0x28;   /* general IO enable pull-down */

  TS_Y1_IOCON   = 0x1A0;   /* general IO disable pulls */
  TS_Y2_IOCON   = 0x20;    /* general IO disable pulls */

  TS_X1_FDIR &= ~TS_X1_MASK;
  TS_X2_FDIR &= ~TS_X2_MASK;
  TS_Y1_FDIR |=  TS_Y1_MASK;
  TS_Y2_FDIR |=  TS_Y2_MASK;

  TS_Y1_FSET  =  TS_Y1_MASK;
  TS_Y2_FSET  =  TS_Y2_MASK;

  /* Init Port interrupt */
  TS_X2_INTR_R  &= ~TS_X2_MASK;   /* disable X2 rising edge interrupt */
  TS_X2_INTR_CLR =  TS_X2_MASK;
  NVIC_EnableIRQ(GPIO_IRQn);

  /* Init ADC */
  LPC_SC->PCONP |= (1<<12);                               /* Enable ADC clocks */
  LPC_ADC->CR = (1<<1) |                                  /* Channel 1 */
                (((PeripheralClock/500000) & 0xFF)<<8) |  /* Set Clock divider*/
                (0<<16)|                                  /* Disable burst*/
                (1<<21)|                                  /* A/D converter is operational */
                (0<<24);                                  /* No Start */
    
  // clear all pending interrupts
  while(LPC_ADC->STAT & (1<<16))
  {
    volatile uint32_t Dummy = LPC_ADC->GDR;
  }

  
  LPC_ADC->INTEN = (1<<8);      /* Enable global interrupt*/
  NVIC_EnableIRQ(ADC_IRQn);

  /* Init delay timer */
  LPC_SC->PCONP |= (1<<2);      /* Enable TIM1 clocks */
  LPC_TIM1->TCR = (1<<1);       /* counting  disable, set reset */
  LPC_TIM1->TCR = 0;            /* release reset */
  LPC_TIM1->CTCR = 0;           /* Timer Mode: every rising PCLK edge */
  LPC_TIM1->MCR = (1<<0) |      /* Enable Interrupt on MR0*/
                  (1<<1) |      /* Enable reset on MR0 */
                  (1<<2) ;      /* Enable stop on MR0 */
  /*set timer 1 period*/
  LPC_TIM1->PR = (PeripheralClock/ 1000000) - 1;
  LPC_TIM1->MR0 = TS_SETUP_DLY;

  LPC_TIM1->IR = (1<<0);  /* clear pending MR0INT*/
  /*Enable NVIC TMR1 Interrupt*/
  NVIC_EnableIRQ(TIMER1_IRQn);
  LPC_TIM1->TCR = 1;    /* start timer 1 */
}

/*************************************************************************
 * Function Name: TouchScrGetStatus
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: uint32_t 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touch screen
 *
 *************************************************************************/
uint32_t TouchGet (ToushRes_t * pData)
{
uint32_t  TouchResReadyHold = 1;
uint32_t X_coordinate, Y_coordinate;

  if(TouchResReadyHold)
  {
    X_coordinate = 30000;
    Y_coordinate = 40000;
    // Calculate X coordinate in pixels
    if (X_coordinate <= LEFT_UP_X)
    {
      X_coordinate = 0;
    }
    else if (X_coordinate >= RIGHT_UP_X)
    {
      X_coordinate = IMAGE_WIDTH-1;
    }
    else
    {
      X_coordinate = ((X_coordinate - LEFT_UP_X) * IMAGE_WIDTH)/(RIGHT_UP_X-LEFT_UP_X);
    }
    // Calculate Y coordinate in pixels
    if (Y_coordinate <= LEFT_UP_Y)
    {
      Y_coordinate = 0;
    }
    else if (Y_coordinate >= RIGHT_UP_Y)
    {
      Y_coordinate = IMAGE_HEIGHT-1;
    }
    else
    {
      Y_coordinate = ((RIGHT_UP_Y-Y_coordinate) * IMAGE_HEIGHT)/(RIGHT_UP_Y-LEFT_UP_Y);
    }
    pData->X = X_coordinate;
    pData->Y = Y_coordinate;
  }
  return(TouchResReadyHold);
}
