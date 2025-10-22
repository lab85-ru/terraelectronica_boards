/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : LCD.c
 *    Description : IAR-LPC4088-SK LCD Example
 *
 *    1. Date        : September 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial Revision
 *
 * This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for IAR-LPC-4088-SK board. It shows basic use of I/O,
 * timer, interrupt and LCD controllers.
 *
 * The IAR Embedded Workbench logo appear on the LCD. The cursor moves as 
 * the board position is changed(the acceleration sensor is used).
 * 
 * Jumpers:
 *  PWR_SEL    - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 * 
 * Note:
 *  The LCD shares pins with Trace port. If ETM is enabled the LCD will not
 *  work.
 *
 *    $Revision: 59014 $
 **************************************************************************/
#include <yfuns.h>
#include <stdint.h>

#include "LPC407x_8x.h"
#include "board.h"
#include "sdram_64M_32bit_drv.h"
#include "drv_glcd.h"
#include "cursor.h"
#include "tjpgd.h"
#include "jpg1.c"
#include "integer.h"



typedef struct {
        const BYTE * jpic;              //уазатель на массив с JPEG картинкой
        DWORD jsize;                     //размер массива
        DWORD joffset;                   //текущая позиция при чтении массива
} IODEV;
extern DWORD SDRAM_VRAM_BASE_ADDR;
#define LCD_VRAM_BASE_ADDR ((DWORD)&SDRAM_VRAM_BASE_ADDR)

volatile uint32_t Tick;

#pragma section=".intvec"

/** public functions **/
/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: void
 * Return: 
 *
 * Description: Low level system init (clock, flash accelerator, 
 *              SDRAM and vector table address)
 *              
 *
 *************************************************************************/
int __low_level_init (void)
{
  /*Here the clock is already set by
    SystemInit function*/
  
  /*SDRAM init*/
  SDRAM_Init();

  return  1;
}

/*************************************************************************
 * Function Name: TMR0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void TIMER0_IRQHandler (void)
{
  ++Tick;
  /*clear match channel 0 interrupt */
  LPC_TIM0->IR = (1<<0);
  /*Data synchronization*/
  __DSB();
}

/*************************************************************************
 * Function Name: TMR0_Init
 * Parameters: uint32_t tps - Ticks per second
 *
 * Return: none
 *
 * Description: Timer 0 start
 *
 *************************************************************************/
void TMR0_Init(uint32_t tps)
{
  Tick = 0;
  /* Enable TIM0 clock*/
  LPC_SC->PCONP |= (1<<1);

  /*Init Timer 0 */
  LPC_TIM0->TCR = (1<<1);       /* counting  disable, set reset */
  LPC_TIM0->TCR = 0;            /* release reset */
  LPC_TIM0->CTCR = 0;           /* Timer Mode: every rising PCLK edge */
  LPC_TIM0->MCR = (1<<0) |      /* Enable Interrupt on MR0*/
                  (1<<1) |      /* Enable reset on MR0 */
                  (0<<2) ;      /* Disable stop on MR0 */
  /*set timer 0 period*/
  LPC_TIM0->PR = 0;
  LPC_TIM0->MR0 = PeripheralClock/(tps);
  /* init timer 0 interrupt */
  LPC_TIM0->IR = (1<<0);        /* clear pending interrupt */
  LPC_TIM0->TCR = (1<<0);       /* counting Enable */
  /*Enable NVIC TMR0 Interrupt*/
  NVIC_EnableIRQ(TIMER0_IRQn);
}

/*************************************************************************
 * Function Name: TMR0_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 stop
 *
 *************************************************************************/
void TMR0_Stop(void)
{
  /* counting  disable */
  LPC_TIM0->TCR &= ~(1<<0);
  /* Enable NVIC TMR0 Interrupt */
  NVIC_DisableIRQ(TIMER0_IRQn);
  /* Timer 0 Clock disable */
  LPC_SC->PCONP &= ~(1<<1);
}

/**************************************************************************/
//Вывод точки с координатами x,y (нет проверки выхода за рабочую зону)
/**************************************************************************/
void Draw_pix(WORD x, WORD y, WORD pix)
{
  WORD* pDst = (WORD*) LCD_VRAM_BASE_ADDR;
  pDst = (pDst+x)+(y*C_GLCD_H_SIZE);
      *(pDst) = pix;
}
/*************************************************************************/
UINT input_func (JDEC * jd, BYTE * buff, UINT ndata) {
        IODEV * dev = (IODEV *) jd->device;
        //Проверяем выход за границы массива
        ndata = dev->jsize - dev->joffset  > ndata ? ndata : dev->jsize - dev->joffset;
        //Копируем фрагмент из массива в буффер
        if(buff) memcpy(buff, dev->jpic + dev->joffset, ndata);
        //Увеличиваем текущее смещение
        dev->joffset += ndata;
        //Возвращаем реальный размер скопированных данных
        return ndata;
}
/************************************************************************/
UINT output_func(JDEC * jd, void * bitmap, JRECT * rect) {

        WORD * bmp = (WORD *) bitmap;

        WORD x;
        WORD y;
        WORD i = 0;
       for(y = rect->top; y <= rect->bottom; y++) {
                for(x = rect->left; x <= rect->right; x++) {
                        Draw_pix(x,y,bmp[i++]);
                }
        }
        return 1;       //Продолжить декодирование
}
/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
        JDEC jd;
        IODEV iodev;
        JRESULT rc;

        BYTE jdwork[3100];

  /*Disable interrupts*/
  __disable_interrupt();
  /*Disable LCD controller*/
  GLCD_Ctrl (FALSE);
  /*Init LCD*/
  GLCD_Init (NULL, NULL);
  /*Enable LCD*/
  GLCD_Ctrl (TRUE);
  /*Disable cursor*/
  GLCD_Cursor_Dis(0);
  /*Start timer 0 100 tick per second*/ 
  TMR0_Init(100);
  /*Enable interrupts*/
  __enable_interrupt();

  LPC_LCD->CTRL = (1<<0) |  /* disable GLCD controller */
                  (6<<1) |  /* 16 bpp */
                  (1<<5) |  /* TFT panel */
                  (0<<7) |  /* single panel */
                  (1<<8) |  /* BGR output */
                  (0<<9) |  /* little endian byte order */
                  (0<<10)|  /* little endian pix order */
                  (1<<11);  /* disable power */
  
  while(1)
  {
        iodev.jpic = jpg_d0;
        iodev.jsize = sizeof(jpg_d0);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);
 
        iodev.jpic = jpg_d1;
        iodev.jsize = sizeof(jpg_d1);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);

        iodev.jpic = jpg_d2;
        iodev.jsize = sizeof(jpg_d2);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);
 
        iodev.jpic = jpg_d3;
        iodev.jsize = sizeof(jpg_d3);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);

        iodev.jpic = jpg_d4;
        iodev.jsize = sizeof(jpg_d4);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);

        iodev.jpic = jpg_d5;
        iodev.jsize = sizeof(jpg_d5);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);

        iodev.jpic = jpg_d6;
        iodev.jsize = sizeof(jpg_d6);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);

        iodev.jpic = jpg_d7;
        iodev.jsize = sizeof(jpg_d7);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);

        iodev.jpic = jpg_d8;
        iodev.jsize = sizeof(jpg_d8);
        iodev.joffset = 0;

         //Подготовка к декодированию 
        rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
         //Декодирование и вывод
        rc = jd_decomp(&jd, output_func, 0);
  }
}
/** private functions **/
